set(INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/include" ${INCLUDES})

configure_file(../include/build_types.h ../include/build_types.h)

target_compile_definitions(project_warnings INTERFACE uptr=std::unique_ptr uchar=unsigned\ char)

link_libraries(project_options project_warnings)

add_subdirectory(agents)
add_subdirectory(control)
add_subdirectory(frontend)
add_subdirectory(gameplay)
add_subdirectory(include/Mana)
add_subdirectory(oracles)
add_subdirectory(server)
add_subdirectory(yggdrasil)
