set(ALLEGRO_REQUIRED_ADDONS "font" "primitives" "image" "ttf")

include(FindPkgConfig)
pkg_search_module(ALLEGRO REQUIRED IMPORTED_TARGET "allegro-5")

if(NOT ALLEGRO_FOUND)
    FetchContent_Declare(
            ALLEGRO
            GIT_REPOSITORY https://github.com/liballeg/allegro5.git
            GIT_TAG   5.2.8
            CMAKE_ARGS     -DCMAKE_BUILD_TYPE:STRING=Release
    )
    FetchContent_MakeAvailable(Allegro)
    set(ALLEGRO_INCLUDE_DIRS "${ALLEGRO_SOURCE_DIR}/include" "${ALLEGRO_BINARY_DIR}/include")
    set(ALLEGRO_LIBRARIES allegro)

    foreach(addon ${ALLEGRO_REQUIRED_ADDONS})
        list(APPEND ALLEGRO_INCLUDE_DIRS "${ALLEGRO_SOURCE_DIR}/addons/${addon}")
        list(APPEND ALLEGRO_LIBRARIES "allegro_${addon}")
    endforeach()
else()
    foreach(addon ${ALLEGRO_REQUIRED_ADDONS})
        pkg_search_module(${addon} REQUIRED IMPORTED_TARGET "allegro_${addon}-5")
        list(APPEND ALLEGRO_INCLUDE_DIRS ${${addon}_INCLUDE_DIRS})
        list(APPEND ALLEGRO_LIBRARIES ${${addon}_LIBRARIES})
        message(${ADDON_LIBRARIES})
        message(${ALLEGRO_LIBRARIES})
    endforeach()
endif()

add_library(IOAllegro STATIC io_allegro.cpp io_allegro_combat.cpp io_allegro_input.cpp)
message(${ALLEGRO_LIBRARIES})
target_link_libraries(IOAllegro ${ALLEGRO_LIBRARIES} Mana BaseIO)
# both build and src are necessary (I guess build contains configured headers?)
if(ALLEGRO_INCLUDE_DIRS) # it could also need no include dirs, if the headers are in /usr/include
    MESSAGE(${ALLEGRO_INCLUDE_DIRS})
    target_include_directories(IOAllegro PUBLIC ${ALLEGRO_INCLUDE_DIRS})
endif()

create_test(IOAllegro)
if(ENABLE_TESTING)
    target_link_libraries(IOAllegro-test gameplay control BaseFrontEnd)
endif()
