add_library(network-server STATIC async.cpp networkmanager.cpp)
target_link_libraries(network-server PRIVATE network)
