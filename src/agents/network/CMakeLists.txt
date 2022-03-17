add_library(network_agent STATIC networkagent.cpp)
target_link_libraries(network_agent PUBLIC filereader network)
