add_library(control game.cpp phases.cpp takeopts.cpp combat.cpp OutputManager.cpp)

target_link_libraries(control PUBLIC gameplay)
target_include_directories(control PUBLIC ${INCLUDES})