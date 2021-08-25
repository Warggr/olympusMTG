add_library(mock_agent STATIC MockAgent.cpp)
target_include_directories(local_agent PUBLIC ${INCLUDES})
