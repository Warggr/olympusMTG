add_library(local_agent STATIC localagent.cpp)
target_link_libraries(local_agent PUBLIC FrontEnd)
target_include_directories(local_agent PUBLIC ${INCLUDES})