set(IOLibType "Allegro" CACHE STRING "Use Allegro as I/O library")
set_property(CACHE IOLibType PROPERTY STRINGS "Allegro" "Ncurses" "Nano")

add_subdirectory(Allegro)
add_subdirectory(Ncurses)
add_subdirectory(Nano)
string(TOLOWER ${IOLibType} LowerCaseIOLib)
set(LowerCaseIOLib ${LowerCaseIOLib} CACHE INTERNAL "")
