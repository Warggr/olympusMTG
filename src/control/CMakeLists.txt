add_library(control STATIC game.cpp phases.cpp takeopts.cpp combat.cpp OutputManager.cpp)

target_link_libraries(control PUBLIC gameplay filereader)
target_include_directories(control PUBLIC ${INCLUDES})
