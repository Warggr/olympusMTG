set(IOLibType "Allegro" CACHE STRING "Use Allegro as I/O library")
set_property(CACHE IOLibType PROPERTY STRINGS "Allegro" "Ncurses" "Nano")

configure_file(../include/lib3_IO.h ../include/lib3_IO.h)
configure_file(../include/implementIO.h ../include/implementIO.h)

add_subdirectory(Allegro)
add_subdirectory(Ncurses)
add_subdirectory(Nano)
string(TOLOWER ${IOLibType} LowerCaseIOLib)
set(LowerCaseIOLib ${LowerCaseIOLib} CACHE INTERNAL "")
