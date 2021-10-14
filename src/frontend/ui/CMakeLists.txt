set(SOURCES default_ui.cpp ui_element.cpp default_ui_combat.cpp default_ui_iterate.cpp argus.cpp)

add_library(UIDefault STATIC ${SOURCES})
add_library(UIDefaultWithMock STATIC ${SOURCES})
set(UILibType Default PARENT_SCOPE)

target_include_directories(UIDefault PUBLIC ${INCLUDES})
target_include_directories(UIDefaultWithMock PUBLIC ${TEST-INCLUDES})

if(ENABLE_TESTING)
    add_executable(UI-test test.cpp mockio.cpp)
    target_link_libraries(UI-test PRIVATE UIDefaultWithMock Catch2::Catch2WithMain
            PRIVATE project_warnings)
    catch_discover_tests(UI-test)
endif()