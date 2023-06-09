add_library(CLI OBJECT io_nano.cpp io_nano_combat.cpp cliUI.cpp cliUI_terminal.cpp ../disp.cpp)
target_link_libraries(CLI PUBLIC Mana oracles gameplay)
target_include_directories(CLI PUBLIC ${INCLUDES})

find_library(READLINE_LIBRARY readline REQUIRED)
if(READLINE_LIBRARY)
    target_compile_definitions(CLI PRIVATE USE_READLINE)
    target_include_directories(CLI PRIVATE READLINE)
    target_link_libraries(CLI PRIVATE ${READLINE_LIBRARY})
endif()

create_test(CLI)
