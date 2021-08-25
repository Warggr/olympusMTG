add_executable(olympus_client networkclient.cpp ../network.cpp OlympusClient.cpp client.cpp)
target_link_libraries(olympus_client PUBLIC FrontEnd Mana pthread)
target_include_directories(olympus_client PUBLIC ${INCLUDES})
set_target_properties(olympus_client PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${RUNTIME_OUTPUT_DIRECTORY})
