add_subdirectory(permanents)
add_subdirectory(resolvables)
add_subdirectory(description)

add_library(gameplay STATIC
        abilities.cpp cards.cpp castopts.cpp modifiers.cpp triggers.cpp ueot.cpp)

target_link_libraries(gameplay PUBLIC permanents resolvables yggdrasil oracles Mana)
target_link_libraries(gameplay PRIVATE project_warnings)

create_test(gameplay)
