set(INCLUDES ${INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${PROJECT_BINARY_DIR}/src/frontend/include)
set(TEST-INCLUDES ${INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${PROJECT_BINARY_DIR}/src/frontend/test-include)

add_subdirectory(io)
add_subdirectory(ui)

add_library(FrontEnd STATIC frontend.cpp menuscreen.cpp)
target_include_directories(FrontEnd PUBLIC ${INCLUDES})

target_link_libraries(FrontEnd PUBLIC UI${UILibType})
target_link_libraries(FrontEnd PUBLIC IO)

add_executable(FrontEnd-Test test.cpp)
target_link_libraries(FrontEnd-Test PRIVATE FrontEnd Catch2::Catch2WithMain)
catch_discover_tests(FrontEnd-Test)