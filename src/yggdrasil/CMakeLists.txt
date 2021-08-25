add_library(yggdrasil STATIC archetypes.cpp board_n.cpp)
target_include_directories(yggdrasil PUBLIC ${INCLUDES})
target_link_libraries(yggdrasil PRIVATE project_warnings)