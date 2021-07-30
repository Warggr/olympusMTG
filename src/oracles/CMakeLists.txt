add_library(Oracles
        filereader.cpp rules_text.cpp readwords.cpp binaryreader.cpp reader.cpp binarywriter.cpp classes/describe.cpp)
target_include_directories(Oracles PUBLIC ${INCLUDES})

add_subdirectory(Dictionary)
target_link_libraries(Oracles PUBLIC Dictionary)