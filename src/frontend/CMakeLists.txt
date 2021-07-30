add_subdirectory(io)
add_subdirectory(ui)

add_library(FrontEnd frontend.cpp menuscreen.cpp)
target_include_directories(FrontEnd PUBLIC ${INCLUDES} "io/${IOLibDir}")
target_link_libraries(FrontEnd PUBLIC IO${IOLibType})
target_link_libraries(FrontEnd PUBLIC UI${UILibType})
