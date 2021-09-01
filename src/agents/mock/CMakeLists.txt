add_library(mock_agent STATIC MockAgent.cpp)
target_include_directories(mock_agent PUBLIC ${INCLUDES})
target_compile_definitions(mock_agent PUBLIC MOCK_AGENTS)