set(DictionaryType "StdMap" CACHE STRING "Use std::map or a custom dictionary")
set_property(CACHE DictionaryType PROPERTY STRINGS "Olympic" "StdMap")

configure_file(include/lib1_dict.h include/lib1_dict.h)

message(STATUS "Choosing ${DictionaryType} as a dict library")

add_subdirectory(Olympic)
add_subdirectory(StdMap)

add_library(Dictionary dictholder.cpp)
target_link_libraries(Dictionary PUBLIC ${DictionaryType}Dict)
target_include_directories(Dictionary PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/include )

add_executable(Dictionary-test test.cpp)
target_link_libraries(Dictionary-test PUBLIC Dictionary)
