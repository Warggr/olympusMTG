add_executable(olympus_server main.cpp server.cpp)
target_include_directories(olympus_server PUBLIC ${INCLUDES})
target_link_libraries(olympus_server PUBLIC agents)