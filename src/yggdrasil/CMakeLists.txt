add_library(yggdrasil STATIC board_n.cpp)
target_include_directories(yggdrasil PUBLIC ${INCLUDES})

create_test(yggdrasil)
if(ENABLE_TESTING)
    target_link_libraries(yggdrasil-test gameplay)
endif()
