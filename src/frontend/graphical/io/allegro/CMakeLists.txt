add_library(IOAllegro STATIC io_allegro.cpp io_allegro_combat.cpp io_allegro_input.cpp)
target_link_libraries(IOAllegro allegro allegro_font allegro_primitives allegro_image allegro_ttf Mana)
target_include_directories(IOAllegro PUBLIC ${INCLUDES})

create_test(IOAllegro)
