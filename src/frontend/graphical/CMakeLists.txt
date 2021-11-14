add_subdirectory(io)
add_subdirectory(windows)

add_library(GUI STATIC graphical_ui.cpp)
target_link_libraries(GUI PRIVATE IO UIDefault)

create_test(GUI)
