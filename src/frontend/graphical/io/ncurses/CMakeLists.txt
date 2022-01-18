add_library(IONcurses STATIC io_ncurses.cpp io_ncurses_combat.cpp)
target_link_libraries(IONcurses ncurses Mana)

create_test(IONcurses)
if(ENABLE_TESTING)
    target_link_libraries(IONcurses-test IO control gameplay_description FrontEnd)
endif()
