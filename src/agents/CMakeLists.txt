add_subdirectory(bot)
add_subdirectory(network)
add_subdirectory(local)

add_library(agents STATIC agent.cpp)
target_include_directories(agents PUBLIC ${INCLUDES})
target_link_libraries(agents PRIVATE local_agent)
target_link_libraries(agents PRIVATE network_agent)
target_link_libraries(agents PRIVATE pthread)