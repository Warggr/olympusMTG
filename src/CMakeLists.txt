set(INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/include" ${INCLUDES})

configure_file(../include/build_types.h ../include/build_types.h)

add_subdirectory(agents)
add_subdirectory(control)
add_subdirectory(frontend)
add_subdirectory(gameplay)
add_subdirectory(include/Mana)
add_subdirectory(oracles)
add_subdirectory(server)
add_subdirectory(yggdrasil)

configure_file(../include/lib3_IO.h ../include/lib3_IO.h)
