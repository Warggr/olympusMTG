set(DictionaryType "StdMap" CACHE STRING "Use std::map or a custom dictionary")
set_property(CACHE DictionaryType PROPERTY STRINGS "Olympic" "StdMap")

configure_file(config/lib1_dict.h config/lib1_dict.h)

message(STATUS "Choosing ${DictionaryType} as a dict library")

add_subdirectory(Olympic)
add_subdirectory(StdMap)

add_library(dictionary dictholder.cpp)
target_link_libraries(dictionary PUBLIC ${DictionaryType}Dict)
target_include_directories(dictionary PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/config )

create_test(dictionary)
