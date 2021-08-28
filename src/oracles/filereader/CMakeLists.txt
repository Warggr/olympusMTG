add_library(filereader STATIC
        binaryreader.cpp binarywriter.cpp filereader.cpp reader.cpp readwords.cpp readfile.cpp rules_text.cpp
        constants.cpp createopts.cpp ../../gameplay/castopts.cpp)
target_include_directories(filereader PUBLIC ${INCLUDES})
target_include_directories(filereader PRIVATE ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(filereader PRIVATE leveldb PUBLIC Mana
        PRIVATE project_warnings)

add_subdirectory(Dictionary)