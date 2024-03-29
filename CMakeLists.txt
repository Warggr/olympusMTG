cmake_minimum_required(VERSION 3.19 FATAL_ERROR)
set(PROJECT_VERSION_MAJOR 2)
set(PROJECT_VERSION_MINOR 0)
set(PROJECT_VERSION_PATCH 0)
set(PROJECT_VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH})

project(olympus VERSION ${PROJECT_VERSION} LANGUAGES CXX)

if ( ${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR} )
    message( FATAL_ERROR "In-source builds not allowed! Create a build directory and run CMake from there." )
endif()

#options
option(LIBCPP "Build with libc++" FALSE)
option(ENABLE_COVERAGE "Enable coverage reporting for gcc/clang" FALSE)
option(BUILD_SHARED_LIBS "Enable compilation of shared libraries" FALSE)
option(ENABLE_TESTING "Enable the building of the test" TRUE)
option(ENABLE_CPPCHECK "Enable testing with cppcheck" FALSE)
option(SIMPLE_BUILD "Build the project as minimally as possible" FALSE)
option(BUILD_DOC "Build the project's documentation" ON)
option(FORCE_COLORED_OUTPUT "Always produce ANSI-colored output (GNU/Clang only)." TRUE)
option(DEBUG_LOGGING "Enabling debug logging" FALSE)
option(ENABLE_PROFILING "Enabling profiling of code with gprof" FALSE)

set(SanitizeType "None" CACHE STRING "What sanitizers to use")
set_property(CACHE SanitizeType PROPERTY STRINGS "None" "adress" "thread" "undefined" "leak" "memory")

set(DEFAULT_BUILD_TYPE "Debug")

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to '${DEFAULT_BUILD_TYPE}' as none was specified.")
    set(CMAKE_BUILD_TYPE "${DEFAULT_BUILD_TYPE}" CACHE STRING "Choose the type of build." FORCE)
    # Set the possible values of build type for cmake-gui
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

#add interfaces
add_library(project_warnings INTERFACE)
add_library(project_options INTERFACE)

target_compile_features(project_options INTERFACE cxx_std_20) #20 needed for C-style struct initializer
if(NOT SanitizeType STREQUAL "None")
    target_compile_options(project_options INTERFACE -fsanitize=${SanitizeType})
    target_link_libraries(project_options INTERFACE -fsanitize=${SanitizeType})
endif()

if(ONLY_COVERAGE OR ENABLE_COVERAGE)
    target_compile_options(project_options INTERFACE --coverage -O0 -g)
    target_link_libraries(project_options INTERFACE --coverage)
endif()

if(ENABLE_PROFILING)
    target_compile_options(project_options INTERFACE -pg)
endif()

#Warnings
target_compile_options(project_warnings INTERFACE -Wall -Wextra
        -Wcast-align
        -Wunused -Wpedantic
        -Wdouble-promotion

#Specific to C++
    $<$<COMPILE_LANGUAGE:CXX>:
        -Woverloaded-virtual -Wnon-virtual-dtor
#        -Wformat=2
    >

#Specific to GCC
    $<$<CXX_COMPILER_ID:GNU>:
        -Wmisleading-indentation
        -Wduplicated-cond
        -Wlogical-op
        $<$<COMPILE_LANGUAGE:CXX>:
            -Wuseless-cast
        >
    >
)

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    target_compile_options(project_warnings INTERFACE $<$<COMPILE_LANGUAGE:CXX>: -Werror -Wnull-dereference>)
endif()

#colored output
if (${FORCE_COLORED_OUTPUT})
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        target_compile_options (project_options INTERFACE -fdiagnostics-color=always)
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        target_compile_options (project_options INTERFACE -fcolor-diagnostics)
    endif()
endif()

if(ENABLE_CPPCHECK)
    find_program(CPPCHECK cppcheck)
    if(CPPCHECK)
        set(CMAKE_CXX_CPPCHECK
                ${CPPCHECK}
                --suppress=syntaxError
                --enable=all
                --inconclusive)
    else()
        message(SEND_ERROR "cppcheck requested but executable not found")
    endif()
endif()

if(ENABLE_CLANG_TIDY)
    find_program(CLANGTIDY clang-tidy)
    if(CLANGTIDY)
        set(CMAKE_CXX_CLANG_TIDY ${CLANGTIDY})
    else()
        message(SEND_ERROR "clang-tidy requested but executable not found")
    endif()
endif()

include(FetchContent)

FetchContent_Declare(
        leveldb
        GIT_REPOSITORY https://github.com/google/leveldb
        GIT_TAG   1.23
        FIND_PACKAGE_ARGS NAMES leveldb
)
FetchContent_MakeAvailable(leveldb)

if(ENABLE_TESTING)
    enable_testing()

    FetchContent_Declare(
            Catch2
            GIT_REPOSITORY https://github.com/catchorg/Catch2.git
            GIT_TAG        v3.1.0
            FIND_PACKAGE_ARGS NAMES Catch2
            CMAKE_ARGS     -DCMAKE_BUILD_TYPE:STRING=Release
    )
    FetchContent_MakeAvailable(Catch2)

    include(CTest)
    include(Catch)
endif()

set(WANTED_DEBUG_FLAGS "DBG_NONE" CACHE STRING "")

set(RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory(src)

set(CONFIGURED_ONCE TRUE CACHE INTERNAL
        "A flag showing that CMake has configured at least once.")
