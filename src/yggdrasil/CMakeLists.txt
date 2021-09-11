add_library(yggdrasil STATIC board_n.cpp)
target_include_directories(yggdrasil PUBLIC ${INCLUDES})

create_test(yggdrasil)
target_link_libraries(yggdrasil-test PRIVATE gameplay)