add_library(permanents STATIC permanents.cpp creature.cpp)
target_include_directories(permanents PUBLIC ${INCLUDES})
target_link_libraries(permanents PRIVATE project_warnings)