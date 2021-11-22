add_library(database STATIC database.cpp)
target_include_directories(database PRIVATE ${INCLUDES})
target_link_libraries(database PUBLIC leveldb PRIVATE filereader dictionary)
