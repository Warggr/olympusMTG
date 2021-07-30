add_library(IONano STATIC io_nano.cpp io_nano_combat.cpp)
target_link_libraries(IONano Mana)
target_include_directories(IONano PUBLIC ${INCLUDES})