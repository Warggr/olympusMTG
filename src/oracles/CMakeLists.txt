add_library(oracles STATIC describe.cpp constants.cpp lifecycle.cpp)

add_subdirectory(filereader)
add_subdirectory(database)

create_test(oracles)
