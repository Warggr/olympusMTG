add_library(resolvables STATIC ../abilities.cpp stack.cpp resolvables.cpp)
target_include_directories(resolvables PUBLIC ${INCLUDES})