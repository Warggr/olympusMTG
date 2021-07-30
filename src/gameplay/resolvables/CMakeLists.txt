add_library(resolvables STATIC ../abilities.cpp stack.cpp castopts.cpp)
target_include_directories(resolvables PUBLIC ${INCLUDES})