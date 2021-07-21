add_library(network_agent STATIC server.cpp networkmanager.cpp network.cpp)
target_include_directories(network_agent PUBLIC ${INCLUDES})

add_executable(olympus_client client.cpp network.cpp)