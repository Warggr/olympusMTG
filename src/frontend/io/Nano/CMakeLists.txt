add_library(IONano OBJECT io_nano.cpp io_nano_combat.cpp)
target_link_libraries(IONano PUBLIC Mana)
target_include_directories(IONano PUBLIC ${INCLUDES})