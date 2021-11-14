find_package( Boost 1.40 COMPONENTS program_options REQUIRED )

add_executable(olympus_client networkclient.cpp OlympusClient.cpp client.cpp)
target_link_libraries(olympus_client PUBLIC agents gameplay ${Boost_LIBRARIES})
target_link_libraries(olympus_client PRIVATE pthread)
set_target_properties(olympus_client PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${RUNTIME_OUTPUT_DIRECTORY})
