cmake_minimum_required(VERSION 3.13 FATAL_ERROR)
project(olympus VERSION 1.0 LANGUAGES CXX)
set(DEFAULT_BUILD_TYPE "Debug")
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to '${DEFAULT_BUILD_TYPE}' as none was specified.")
  set(CMAKE_BUILD_TYPE "${DEFAULT_BUILD_TYPE}" CACHE STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

include_directories(source/HFiles)
file(GLOB SOURCES "source/CFiles/*.cpp")

set(IOLib "allegro" CACHE STRING "Use Allegro as I/O library")
set_property(CACHE IOLib PROPERTY STRINGS allegro ncurses)

 if(IOLib STREQUAL "allegro")
    list(APPEND EXTRA_LIBS allegro allegro_font allegro_primitives allegro_image allegro_ttf)
elseif(IOLib EQUAL "ncurses")
    list(APPEND EXTRA_LIBS ncurses)
endif()

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR
    "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(warnings "-Wall")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    set(warnings "/W4 /WX /EHsc")
endif()
if (NOT CONFIGURED_ONCE)
    set(CMAKE_CXX_FLAGS "${warnings}"
        CACHE STRING "Flags used by the compiler during all build types." FORCE)
    set(CMAKE_C_FLAGS   "${warnings}"
        CACHE STRING "Flags used by the compiler during all build types." FORCE)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
endif() 

configure_file(olympus_main.h.in source/HFiles/olympus_main.h)

add_subdirectory(source/Dictionary)

add_executable(${PROJECT_NAME} ${SOURCES})
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ..)

target_link_libraries(olympus PUBLIC OlympicDict)
target_link_libraries(olympus PUBLIC ${EXTRA_LIBS})

target_include_directories(olympus PUBLIC
	"${PROJECT_BINARY_DIR}"
)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CONFIGURED_ONCE TRUE CACHE INTERNAL
    "A flag showing that CMake has configured at least once.")