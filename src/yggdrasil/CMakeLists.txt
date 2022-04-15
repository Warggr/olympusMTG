add_library(yggdrasil STATIC board_n.cpp)

create_test(yggdrasil)
if(ENABLE_TESTING)
    target_link_libraries(yggdrasil-test gameplay)
endif()
