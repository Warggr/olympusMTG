add_library(Mana STATIC mana.cpp)
target_link_libraries(Mana PRIVATE project_warnings)

add_executable(Mana-test test.cpp)
target_link_libraries(Mana-test PRIVATE Catch2::Catch2WithMain
        PUBLIC Mana)
catch_discover_tests(Mana-test)