add_library(network-client STATIC networkclient.cpp)
target_link_libraries(network-client PRIVATE network)
