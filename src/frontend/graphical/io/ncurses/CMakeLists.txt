add_library(IONcurses STATIC io_ncurses.cpp io_ncurses_combat.cpp)
target_link_libraries(IONcurses ncurses)
target_include_directories(IONcurses PUBLIC ${INCLUDES})
