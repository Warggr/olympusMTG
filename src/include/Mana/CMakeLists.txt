add_library(Mana STATIC mana.cpp)
target_link_libraries(Mana PRIVATE project_warnings)

create_test(Mana)
