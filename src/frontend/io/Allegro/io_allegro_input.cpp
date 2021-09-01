#include "lib3_allegroIO.h"

bool AllegroIO::simpleChoice(const char *optTrue, const char *optFalse) {
    bool current = true;
    while(true) {
        draw_boxed_text(optTrue, current ? HIGH1 : WHITE, BLACK, messageY, messageZ, 500);
        draw_boxed_text(optFalse, current ? WHITE : HIGH1, BLACK, messageY, messageZ + 25, 500);
        DirectioL dir = get_direction_key();
        switch(dir) {
            case UP:
            case DOWN:
                current = !current; break;
            case ENTER:
                return current;
            case MOUSE: //TODO
            default:
                break;
        }
    }
}

//Adapted from https://www.allegro.cc/forums/thread/617234
// and https://github.com/leorising14/New/blob/master/main.c
std::string AllegroIO::getTextInput(const char* question) {
    ALLEGRO_USTR* str = al_ustr_new("Type something...");
    message(question);
    int pos = strlen("Type something...");
    while(true) {
        al_draw_filled_rectangle(messageY, messageZ+25, messageY + 500, messageZ + 50, registeredColors[1]);
        al_draw_ustr(fonts[0], al_map_rgb_f(1, 1, 1), messageY, messageZ+25, 0, str);
        al_flip_display();
        ALLEGRO_EVENT ev;
        al_wait_for_event(queue, &ev);
        if(ev.type == ALLEGRO_EVENT_KEY_CHAR) {
            if(ev.keyboard.unichar >= 32) {
                pos += al_ustr_append_chr(str, ev.keyboard.unichar);
            } else {
                switch(ev.keyboard.keycode) {
                    case ALLEGRO_KEY_ENTER:
                        goto end_function;
                    case ALLEGRO_KEY_BACKSPACE:
                        if(al_ustr_prev(str, &pos))
                            al_ustr_truncate(str, pos);
                        break;
                }
            }
        } else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            throw UIClosedException();
        }
    };
    end_function:
    std::string ret(al_cstr(str));
    al_ustr_free(str);
    return ret;
}

DirectioL AllegroIO::get_direction_key(){
    while(true){
        refresh_display();
        ALLEGRO_EVENT event;
        al_wait_for_event(queue, &event);
        switch(event.type){
            case ALLEGRO_EVENT_KEY_CHAR:
                mouseActive = false;
                switch(event.keyboard.keycode){
                    case ALLEGRO_KEY_DOWN: return DOWN;
                    case ALLEGRO_KEY_UP: return UP;
                    case ALLEGRO_KEY_ENTER: return ENTER;
                    case ALLEGRO_KEY_SPACE: return BACK;
                    case ALLEGRO_KEY_RIGHT: return RIGHT;
                    case ALLEGRO_KEY_LEFT: return LEFT;
                    default: return NOT_RECOGNIZED;
                } break;
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                throw UIClosedException();
            case ALLEGRO_EVENT_MOUSE_AXES:
                mouseActive = true;
                mousey = event.mouse.x; mousez = event.mouse.y;
                return MOUSE;
            case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
                mouseActive = true;
                return ENTER;
        }
    }
}
