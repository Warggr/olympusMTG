add_library(network_agent STATIC server.cpp networkmanager.cpp network.cpp networkwriter.cpp)
target_include_directories(network_agent PUBLIC ${INCLUDES})
target_link_libraries(network_agent PRIVATE project_warnings)

add_subdirectory(client)
