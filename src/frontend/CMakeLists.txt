set(UIType "Graphical" CACHE STRING "Use a graphical or terminal-based user interface")
set_property(CACHE UIType PROPERTY STRINGS "Graphical" "Cli")

string(TOLOWER ${UIType} LowerCaseUIType)
set(LowerCaseUIType ${LowerCaseUIType} CACHE INTERNAL "")

configure_file(include/implementUI.h include/implementUI.h)
configure_file(include/nameUI.h include/nameUI.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/include)

message(STATUS "Choosing ${UIType} as an UI type")

add_subdirectory(graphical)
add_subdirectory(cli)

add_library(FrontEnd STATIC frontend.cpp disp.cpp)

target_link_libraries(FrontEnd PRIVATE CLI GUI)
target_include_directories(FrontEnd PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/include)

create_test(FrontEnd)
