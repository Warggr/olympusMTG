add_subdirectory(Dictionary)

add_library(plain_reader STATIC filereader.cpp readwords.cpp rules_text.cpp)
target_link_libraries(plain_reader PUBLIC Dictionary)
