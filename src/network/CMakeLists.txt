add_library(network STATIC network.cpp)

add_subdirectory(client)
add_subdirectory(server)

create_test(network)

if(ENABLE_TESTING)
    target_link_libraries(network-test network-client network-server pthread)
endif()
