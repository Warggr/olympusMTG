set(IOLibType "Allegro" CACHE STRING "Use Allegro as I/O library")
set_property(CACHE IOLibType PROPERTY STRINGS "Allegro" "Ncurses")

string(TOLOWER ${IOLibType} LowerCaseIOLib)
set(LowerCaseIOLib ${LowerCaseIOLib} CACHE INTERNAL "")

configure_file(include/nameIO.h include/nameIO.h)
configure_file(include/implementIO.h include/implementIO.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/include)

message(STATUS "Choosing ${IOLibType} as an I/O library")

add_library(IO STATIC abstractIO.cpp)
target_include_directories(IO PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/include)

create_test(FrontEnd)

add_subdirectory(allegro)
add_subdirectory(ncurses)

target_link_libraries(IO PRIVATE IOAllegro IONcurses)
