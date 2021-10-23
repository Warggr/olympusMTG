add_library(CLI OBJECT io_nano.cpp io_nano_combat.cpp cliUI.cpp cliUI_terminal.cpp)
target_link_libraries(CLI PUBLIC Mana)
target_include_directories(CLI PUBLIC ${INCLUDES})
