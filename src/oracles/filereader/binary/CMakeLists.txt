option(F_CANARY "Put canaries in the binary data")

add_library(binary_reader STATIC binaryreader.cpp)
add_library(binary_writer STATIC binarywriter.cpp)

configure_file(config.h ${CMAKE_BINARY_DIR}/src/config/binaryvisitor.h)

message(${CMAKE_BINARY_DIR})

if(F_CANARY)
	target_compile_definitions(binary_reader PRIVATE F_CANARY)
	target_compile_definitions(binary_writer PRIVATE F_CANARY)
endif()
