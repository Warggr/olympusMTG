add_library(basic_reader OBJECT reader.cpp)

add_subdirectory(plain)
add_subdirectory(binary)
add_subdirectory(json)

add_library(filereader STATIC readfile.cpp)
target_link_libraries(filereader PRIVATE database binary_writer binary_reader plain_reader dictionary)
