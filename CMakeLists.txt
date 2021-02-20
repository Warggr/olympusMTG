#Intro
cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

set(PROJECT_VERSION_MAJOR 1)
set(PROJECT_VERSION_MINOR 0)
set(PROJECT_VERSION_PATCH 0)
set(PROJECT_VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH})

project(olympus VERSION 1.0 LANGUAGES CXX)

#options
option(ONLY_COVERAGE "Build only tests necessary for coverage" FALSE)
option(LIBCPP "Build with libc++" FALSE)
option(ENABLE_COVERAGE "Enable coverage reporting for gcc/clang" FALSE)
option(ENABLE_ASAN "Enable address sanitizer" FALSE)
option(BUILD_SHARED_LIBS "Enable compilation of shared libraries" FALSE)
option(ENABLE_TESTING "Enable the building of the test" FALSE)
option(ENABLE_CPPCHECK "Enable testing with cppcheck" FALSE)
option(SIMPLE_BUILD "Build the project as minimally as possible" FALSE)
option(BUILD_DOC "Build the project's documentation" ON)
option(FORCE_COLORED_OUTPUT "Always produce ANSI-colored output (GNU/Clang only)." TRUE)
option(DEBUG_LOGGING "Enabling debug logging" FALSE)

#add interfaces
add_library(project_warnings INTERFACE)
add_library(project_options INTERFACE)

if ( ${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR} )
    message( FATAL_ERROR "In-source builds not allowed! Create a build directory and run CMake from there." )
endif()

set(DEFAULT_BUILD_TYPE "Debug")

target_compile_features(project_options INTERFACE cxx_std_17)
if(ENABLE_ASAN)
    target_compile_options(project_options INTERFACE -fsanitize=address)
    target_link_libraries(project_options INTERFACE -fsanitize=address)
endif()

if(ONLY_COVERAGE OR ENABLE_COVERAGE)
    target_compile_options(project_options INTERFACE --coverage -O0 -g)
    target_link_libraries(project_options INTERFACE --coverage)
endif()

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to '${DEFAULT_BUILD_TYPE}' as none was specified.")
  set(CMAKE_BUILD_TYPE "${DEFAULT_BUILD_TYPE}" CACHE STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

#Warnings
target_compile_options(project_warnings INTERFACE -Wall -Wextra -Wshadow
    -Wnon-virtual-dtor -Wold-style-cast -Wcast-align
    -Wunused -Woverloaded-virtual -Wpedantic -Wconversion
    -Wsign-conversion
    -Wnull-dereference -Wdouble-promotion
    -Wformat=2)

# some GCC specific warnings. These flags are added only if the used compiler is GCC.
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    target_compile_options(project_warnings
        INTERFACE
        -Wmisleading-indentation
        -Wduplicated-cond
        -Wlogical-op
        -Wuseless-cast )
    target_link_libraries(project_options INTERFACE stdc++fs)
endif()

#colored output
if (${FORCE_COLORED_OUTPUT})
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        target_compile_options (project_options INTERFACE -fdiagnostics-color=always)
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        target_compile_options (project_options INTERFACE -fcolor-diagnostics)
    endif ()
endif ()

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

if(ENABLE_TESTING)
    enable_testing()
    set(F_TESTS 1)
endif()

set(HEADER_DIR "${PROJECT_SOURCE_DIR}/include")
set(SOURCE_DIR "${PROJECT_SOURCE_DIR}/CFiles")
configure_file(${SOURCE_DIR}/header_link.h ${SOURCE_DIR}/.header_link.h)

include_directories($HEADER_DIR)
file(GLOB SOURCES "${SOURCE_DIR}/*.cpp")

set(IOLibType "Allegro" CACHE STRING "Use Allegro as I/O library")
set_property(CACHE IOLibType PROPERTY STRINGS "Allegro" "Ncurses" "Nano")

add_subdirectory(Dictionary)
add_subdirectory(IOLib)
add_subdirectory(UILib)
add_subdirectory(Mana)
add_subdirectory(Yggdrasil)

if (NOT CONFIGURED_ONCE)
    set(CMAKE_CXX_FLAGS "${warnings}"
        CACHE STRING "Flags used by the compiler during all build types." FORCE)
    set(CMAKE_C_FLAGS   "${warnings}"
        CACHE STRING "Flags used by the compiler during all build types." FORCE)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
endif()

configure_file(${HEADER_DIR}/olympus_main.h ${HEADER_DIR}/.olympus_main.h)

add_executable(${PROJECT_NAME} ${SOURCES} CFiles/options_utils.cpp CFiles/headU_utilities.h)
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ..)

target_link_libraries(olympus PUBLIC OlympicDict)
target_link_libraries(olympus PUBLIC Mana)
target_link_libraries(olympus PUBLIC Yggdrasil)
target_link_libraries(olympus PUBLIC ${UILibDir})
target_link_libraries(olympus PUBLIC ${IOLibDir})

target_include_directories(olympus PUBLIC
	"${PROJECT_BINARY_DIR}"
)

set(CONFIGURED_ONCE TRUE CACHE INTERNAL
    "A flag showing that CMake has configured at least once.")
