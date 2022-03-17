add_library(network STATIC network.cpp networkmanager.cpp async.cpp)
create_test(network)

add_subdirectory(client)
if(ENABLE_TESTING)
    target_link_libraries(network-test pthread)
endif()
