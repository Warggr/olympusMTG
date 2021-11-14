add_library(control STATIC combat.cpp game.cpp phases.cpp takeopts.cpp OutputManager.cpp optionmanager.cpp)

target_link_libraries(control PUBLIC gameplay filereader)
target_include_directories(control PUBLIC ${INCLUDES})
