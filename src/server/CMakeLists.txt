find_package( Boost 1.40 COMPONENTS program_options REQUIRED )

add_executable(olympus_server main.cpp server.cpp)
target_link_libraries(olympus_server agents control pthread ${Boost_LIBRARIES})
set_target_properties(olympus_server PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${RUNTIME_OUTPUT_DIRECTORY})

if(ENABLE_TESTING)
    add_executable(server-test test.cpp)
    target_include_directories(server-test PUBLIC ${INCLUDES})
    target_link_libraries(server-test PRIVATE agents-mock-only control)
    catch_discover_tests(server-test)
endif()
