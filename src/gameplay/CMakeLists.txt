add_subdirectory(permanents)
add_subdirectory(resolvables)

add_library(gameplay STATIC abilities.cpp cards.cpp triggers.cpp modifiers.cpp describe.cpp)
target_include_directories(gameplay PUBLIC ${INCLUDES})

target_link_libraries(gameplay PUBLIC permanents resolvables yggdrasil oracles)
