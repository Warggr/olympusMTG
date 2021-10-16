set(SOURCES ui_element.cpp argus.cpp)

add_library(UIDefault STATIC ${SOURCES})
add_library(UIDefaultWithMock STATIC ${SOURCES})

target_include_directories(UIDefaultWithMock PUBLIC ${TEST-INCLUDES})

target_link_libraries(UIDefault PRIVATE IO)

if(ENABLE_TESTING)
    add_executable(UI-test ../io/mock/mockio.cpp)
    target_link_libraries(UI-test PRIVATE UIDefaultWithMock Catch2::Catch2WithMain
            PRIVATE project_warnings)
    catch_discover_tests(UI-test)
endif()
