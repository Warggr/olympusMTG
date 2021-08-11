add_library(oracles STATIC describe.cpp filereader/createopts.cpp)
target_include_directories(oracles PUBLIC ${INCLUDES})

add_subdirectory(filereader)
target_link_libraries(oracles PRIVATE filereader)
target_link_libraries(oracles PUBLIC Mana)