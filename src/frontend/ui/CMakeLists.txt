add_library(UIDefault default_ui.cpp ui_element.cpp) #default_ui_iterate.cpp default_ui_combat.cpp)
set(UILibType Default PARENT_SCOPE)

target_include_directories(UIDefault PUBLIC ${INCLUDES})