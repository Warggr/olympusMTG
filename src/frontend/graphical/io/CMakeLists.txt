set(IOLibType "Allegro" CACHE STRING "Use Allegro as I/O library")
set_property(CACHE IOLibType PROPERTY STRINGS "Allegro" "Ncurses")

string(TOLOWER ${IOLibType} LowerCaseIOLib)
set(LowerCaseIOLib ${LowerCaseIOLib} CACHE INTERNAL "")

configure_file(config/nameIO.h config/nameIO.h)
configure_file(config/implementIO.h config/implementIO.h)

message(STATUS "Choosing ${IOLibType} as an I/O library")

add_library(IO STATIC abstractIO.cpp)
target_include_directories(IO PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/config)

add_subdirectory(allegro)
add_subdirectory(ncurses)
add_subdirectory(mock)

target_link_libraries(IO PRIVATE IOAllegro IONcurses)
target_link_libraries(IO PUBLIC oracles gameplay_description)

create_test(IO)
