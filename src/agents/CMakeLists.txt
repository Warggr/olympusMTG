add_subdirectory(bot)
add_subdirectory(network)
add_subdirectory(player)

add_library(agents STATIC agent.cpp)
target_include_directories(agents PUBLIC ${INCLUDES})
target_link_libraries(agents PUBLIC network_agent)
target_link_libraries(agents PUBLIC pthread)