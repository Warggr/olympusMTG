add_library(network_agent STATIC networkagent.cpp networkmanager.cpp network.cpp)
target_include_directories(network_agent PUBLIC ${INCLUDES})

add_subdirectory(client)