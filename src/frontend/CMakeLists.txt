set(INCLUDES ${INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${PROJECT_BINARY_DIR}/src/frontend/include)
set(TEST-INCLUDES ${INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${PROJECT_BINARY_DIR}/src/frontend/test-include)

add_subdirectory(io)
add_subdirectory(ui)

add_library(FrontEnd STATIC frontend.cpp menuscreen.cpp)
target_include_directories(FrontEnd PUBLIC ${INCLUDES})

target_link_libraries(FrontEnd PRIVATE UI${UILibType})
target_link_libraries(FrontEnd PRIVATE IO)

if(ENABLE_TESTING)
    add_executable(FrontEnd-test test.cpp)
    target_include_directories(FrontEnd-test PUBLIC ${TEST-INCLUDES})
    target_link_libraries(FrontEnd-test PRIVATE FrontEnd)
    catch_discover_tests(FrontEnd-test)
endif()