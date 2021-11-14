set(SOURCES ui_element.cpp argus.cpp)

add_library(UIDefault STATIC ${SOURCES})
add_library(UIDefaultWithMock STATIC ${SOURCES})

target_include_directories(UIDefaultWithMock PUBLIC ${TEST-INCLUDES})

target_link_libraries(UIDefault PRIVATE IO)
