add_subdirectory(bot)
add_subdirectory(network)
add_subdirectory(local)
add_subdirectory(mock)

add_library(agents STATIC agent.cpp)
target_link_libraries(agents PRIVATE local_agent network_agent bot_agent)

add_library(agents-mock-only STATIC agent.cpp)
target_include_directories(agents-mock-only PUBLIC ${INCLUDES})
target_link_libraries(agents-mock-only PRIVATE mock_agent)
target_compile_definitions(agents-mock-only PRIVATE MOCK_AGENT)

create_test(agents)
