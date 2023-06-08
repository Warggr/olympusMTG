FetchContent_Declare(
        Allegro
        GIT_REPOSITORY https://github.com/liballeg/allegro5.git
        GIT_TAG   master
        FIND_PACKAGE_ARGS NAMES Allegro
        CMAKE_ARGS     -DCMAKE_BUILD_TYPE:STRING=Release
)
FetchContent_MakeAvailable(Allegro)

add_library(IOAllegro STATIC io_allegro.cpp io_allegro_combat.cpp io_allegro_input.cpp)
target_link_libraries(IOAllegro allegro allegro_font allegro_primitives allegro_image allegro_ttf Mana BaseIO)
# both build and src are necessary (I guess build contains configured headers?)
target_include_directories(IOAllegro PUBLIC ${CMAKE_BINARY_DIR}/_deps/allegro-build/include)
target_include_directories(IOAllegro PUBLIC ${CMAKE_BINARY_DIR}/_deps/allegro-src/include)
target_include_directories(IOAllegro PUBLIC ${CMAKE_BINARY_DIR}/_deps/allegro-src/addons/font)
target_include_directories(IOAllegro PUBLIC ${CMAKE_BINARY_DIR}/_deps/allegro-src/addons/primitives)
target_include_directories(IOAllegro PUBLIC ${CMAKE_BINARY_DIR}/_deps/allegro-src/addons/image)
target_include_directories(IOAllegro PUBLIC ${CMAKE_BINARY_DIR}/_deps/allegro-src/addons/ttf)

create_test(IOAllegro)
if(ENABLE_TESTING)
    target_link_libraries(IOAllegro-test gameplay control BaseFrontEnd)
endif()
