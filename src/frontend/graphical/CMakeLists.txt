add_subdirectory(io)
add_subdirectory(windows)

add_library(GUI STATIC graphical_ui.cpp)
target_link_libraries(GUI PRIVATE IO UIDefault)
target_link_libraries(GUI PUBLIC BaseFrontEnd)

create_test(GUI)
if(ENABLE_TESTING)
    target_link_libraries(GUI-test gameplay control BaseFrontEnd)
endif()
