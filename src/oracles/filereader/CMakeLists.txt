add_library(filereader STATIC
        binaryreader.cpp binarywriter.cpp filereader.cpp reader.cpp readwords.cpp readfile.cpp rules_text.cpp
        createopts.cpp ../../gameplay/castopts.cpp)
target_include_directories(filereader PUBLIC ${INCLUDES})
target_include_directories(filereader PUBLIC ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(Dictionary)

target_link_libraries(filereader PRIVATE database Dictionary PUBLIC Mana)
