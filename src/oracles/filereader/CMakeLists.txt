add_library(filereader STATIC reader.cpp readfile.cpp)
target_include_directories(filereader PUBLIC ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(plain)
add_subdirectory(binary)

target_link_libraries(filereader PRIVATE database binary_writer binary_reader plain_reader)
