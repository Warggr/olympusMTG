add_subdirectory(permanents)
add_subdirectory(resolvables)

add_library(gameplay STATIC cards.cpp triggers.cpp modifiers.cpp)
target_include_directories(gameplay PUBLIC ${INCLUDES})
target_link_libraries(gameplay PUBLIC permanents)
target_link_libraries(gameplay PUBLIC resolvables)