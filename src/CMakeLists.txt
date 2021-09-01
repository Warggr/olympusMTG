set(INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/include" ${INCLUDES})

configure_file(../include/build_types.h ../include/build_types.h)

target_compile_definitions(project_warnings INTERFACE uptr=std::unique_ptr uchar=unsigned\ char flag_t=unsigned\ char)

link_libraries(project_options project_warnings)

function(create_test SUT)
    if(ENABLE_TESTING)
        add_executable(${SUT}-test test.cpp)
        target_include_directories(${SUT}-test PRIVATE ${INCLUDES})
        target_link_libraries(${SUT}-test PRIVATE ${SUT} Catch2::Catch2WithMain)
        target_compile_definitions(${SUT}-test PRIVATE F_TESTS)
        catch_discover_tests(${SUT}-test)
        message(STATUS "Includes for ${SUT}-test: ${INCLUDES}")
    endif()
endfunction()

add_subdirectory(agents)
add_subdirectory(control)
add_subdirectory(frontend)
add_subdirectory(gameplay)
add_subdirectory(include/Mana)
add_subdirectory(oracles)
add_subdirectory(server)
add_subdirectory(yggdrasil)
