add_library(network STATIC network.cpp)
target_include_directories(network PRIVATE ${BOOST_INCLUDE_DIRS})

add_subdirectory(client)
add_subdirectory(server)

create_test(network)

if(ENABLE_TESTING)
    target_link_libraries(network-test network-client network-server pthread)
endif()
