add_library(IOAllegro STATIC io_allegro.cpp io_allegro_combat.cpp io_allegro_input.cpp)
target_link_libraries(IOAllegro allegro allegro_font allegro_primitives allegro_image allegro_ttf Mana BaseIO)

create_test(IOAllegro)
if(ENABLE_TESTING)
    target_link_libraries(IOAllegro-test gameplay control BaseFrontEnd)
endif()