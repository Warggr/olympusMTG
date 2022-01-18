set(IOLibType "Allegro" CACHE STRING "Use Allegro as I/O library")
set_property(CACHE IOLibType PROPERTY STRINGS "Allegro" "Ncurses")
string(TOLOWER ${IOLibType} LowerCaseIOLib)
set(LowerCaseIOLib ${LowerCaseIOLib} CACHE INTERNAL "")
configure_file(config/nameIO.h config/nameIO.h)
configure_file(config/implementIO.h config/implementIO.h)

message(STATUS "Choosing ${IOLibType} as an I/O library")

add_library(BaseIO STATIC baseIO.cpp)
target_link_libraries(BaseIO PUBLIC gameplay_description permanents)
target_include_directories(BaseIO PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/config)

add_subdirectory(allegro)
add_subdirectory(ncurses)
add_subdirectory(mock)

add_library(IO STATIC abstractIO.cpp)
target_link_libraries(IO PRIVATE IOAllegro IONcurses)

create_test(IO)
if(ENABLE_TESTING)
    target_link_libraries(IO-test IO gameplay control BaseFrontEnd BaseIO)
endif()
