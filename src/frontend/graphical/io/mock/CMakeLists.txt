if(ENABLE_TESTING)
    add_library(IOMock STATIC mockio.cpp)
    target_compile_options(IOMock PRIVATE -Wno-unused-parameter)
endif()
