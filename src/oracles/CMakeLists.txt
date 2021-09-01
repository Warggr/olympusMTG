add_library(oracles STATIC describe.cpp filereader/createopts.cpp)
target_include_directories(oracles PUBLIC ${INCLUDES})

add_subdirectory(filereader)
add_subdirectory(database)

target_link_libraries(oracles PUBLIC filereader Mana)
target_link_libraries(oracles PRIVATE project_warnings)