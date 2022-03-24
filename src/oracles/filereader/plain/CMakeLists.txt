add_subdirectory(Dictionary)

add_library(plain_reader STATIC filereader.cpp readwords.cpp rules_text.cpp)
target_link_libraries(plain_reader PUBLIC Mana basic_reader dictionary)

create_test(plain_reader)
if(ENABLE_TESTING)
    target_link_libraries(plain_reader-test dictionary oracles gameplay_description)
endif()
