set(IOLibType "Allegro" CACHE STRING "Use Allegro as I/O library")
set_property(CACHE IOLibType PROPERTY STRINGS "Allegro" "Ncurses" "Nano")

configure_file(../include/lib3_IO.h ../include/lib3_IO.h)
configure_file(../include/implementIO.h ../include/implementIO.h)

message(STATUS "Choosing ${IOLibType} as an I/O library")

add_subdirectory(Allegro)
add_subdirectory(Ncurses)
add_subdirectory(Nano)

add_library(IO STATIC disp.cpp)
target_link_libraries(IO PUBLIC IO${IOLibType})

string(TOLOWER ${IOLibType} LowerCaseIOLib)
set(LowerCaseIOLib ${LowerCaseIOLib} CACHE INTERNAL "")

add_executable(IO-test test.cpp)
target_link_libraries(IO-test PRIVATE Catch2::Catch2WithMain
        PUBLIC IO)
catch_discover_tests(IO-test)
