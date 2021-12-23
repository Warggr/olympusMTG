add_library(IONcurses STATIC io_ncurses.cpp io_ncurses_combat.cpp)
target_link_libraries(IONcurses ncurses)
target_link_libraries(IONcurses Mana)
target_include_directories(IONcurses PUBLIC ${INCLUDES})

create_test(IONcurses)
if(ENABLE_TESTING)
    target_link_libraries(IONcurses-test IO control gameplay_description FrontEnd)
endif()
