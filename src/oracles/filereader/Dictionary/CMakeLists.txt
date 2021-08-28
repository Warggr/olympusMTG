set(DictionaryType "Olympic" CACHE STRING "Use std::map or a custom dictionary")
set_property(CACHE DictionaryType PROPERTY STRINGS "Olympic" "StdMap")

configure_file(lib1_dict.h include/lib1_dict.h)

message(STATUS "Choosing ${DictionaryType} as a dict library")

add_subdirectory(Olympic)
add_subdirectory(StdMap)

add_library(Dictionary INTERFACE)
target_link_libraries(Dictionary INTERFACE Dict${DictionaryType})

#add_executable(IO-test test.cpp)
#target_link_libraries(IO-test PRIVATE Catch2::Catch2WithMain
 #       PUBLIC IO)
#catch_discover_tests(IO-test)