set(DictionaryType "Olympic" CACHE STRING "Use std::map or a custom dictionary")
set_property(CACHE DictionaryType PROPERTY STRINGS "Olympic" "StdMap")

configure_file(include/lib1_dict.h include/lib1_dict.h)
set(INCLUDES ${INCLUDES} ${CMAKE_CURRENT_BINARY_DIR}/include)

message(STATUS "Choosing ${DictionaryType} as a dict library")

add_subdirectory(Olympic)
add_subdirectory(StdMap)

add_library(Dictionary constants.cpp)
target_include_directories(Dictionary PUBLIC ${INCLUDES})
target_link_libraries(Dictionary PUBLIC ${DictionaryType}Dict)

add_executable(Dictionary-test test.cpp)
target_link_libraries(Dictionary-test PUBLIC Dictionary)
#create_test(Dictionary)
