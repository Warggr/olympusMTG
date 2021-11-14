add_library(local_agent OBJECT localagent.cpp local_viewer.cpp)
target_link_libraries(local_agent PUBLIC FrontEnd gameplay)
target_include_directories(local_agent PUBLIC ${INCLUDES})

create_test(local_agent)
