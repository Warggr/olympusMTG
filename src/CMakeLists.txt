set(INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/include" ${INCLUDES})

find_package( Boost 1.40 COMPONENTS log REQUIRED )

configure_file(config/build_types.h config/build_types.h)
configure_file(config/logging.h config/logging.h)

target_compile_definitions(project_warnings INTERFACE
        uptr=std::unique_ptr
        uchar=unsigned\ char
        flag_t=unsigned\ char
        fwdlist=std::forward_list)

link_libraries(project_options project_warnings ${Boost_LIBRARIES})

function(create_test SUT)
    if(ENABLE_TESTING)
        add_executable(${SUT}-test test.cpp)
        target_include_directories(${SUT}-test PRIVATE ${INCLUDES})
        target_link_libraries(${SUT}-test ${SUT} Catch2::Catch2WithMain)
        target_compile_definitions(${SUT}-test PRIVATE F_TESTS)
        catch_discover_tests(${SUT}-test)
    endif()
endfunction()

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_BINARY_DIR}/config)

add_subdirectory(agents)
add_subdirectory(control)
add_subdirectory(client)
add_subdirectory(frontend)
add_subdirectory(gameplay)
add_subdirectory(include/Mana)
add_subdirectory(network)
add_subdirectory(oracles)
add_subdirectory(server)
add_subdirectory(yggdrasil)
