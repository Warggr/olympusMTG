#include "window.h"
#include "lib3_IO.h"

void ModernElement::addElement(UIElement *element, int pos) {
    subWins[pos] = element;
    if(orientation == horizontal) {
        element->coords.y = (pos == 0) ? 0 : endLines[pos-1];
        element->coords.z = coords.z;
        element->coords.height = coords.height;
        endLines[pos] = element->coords.right();
    } else {
        element->coords.z = (pos == 0) ? 0 : endLines[pos-1];
        element->coords.y = coords.y;
        element->coords.height = coords.height;
        endLines[pos] = element->coords.right();
    }
}

void ModernElement::addLastElement(UIElement* element) {
    if(orientation == horizontal) {
        element->coords.y = endLines[subWins.size()-1];
        element->coords.z = coords.z;
        element->coords.height = coords.height;
    } else {
        element->coords.z = endLines[subWins.size()-1];
        element->coords.y = coords.y;
        element->coords.height = coords.height;
    }
    subWins.shrink_to_fit();
}

template<typename T>
Target* Sprite<T>::iterate(char requs, bool needstarget) {
    while (true) {
        direction = ImplementIO::god->get_direction_key();
        switch (direction) {
            case ENTER:
                return target;
            case NOT_RECOGNIZED:
                ImplementIO::god->message("Please press a direction key, SPACE or ENTER");
                break;
            case BACK:
                if(!needstarget) return nullptr;
                break;
            default:
                return nullptr;
        }
    }
}

Target* ModernElement::iterate(char requs, bool needstarget) {
    uint index = 0; if(direction == LEFT || direction == UP) index = subWins.size();
    while(true) {
        Target* tar = subWins[index]->iterate(requs, needstarget);
        if(tar) return tar;
        if((direction == LEFT && orientation == vertical) or (direction == UP && orientation == horizontal) ) {
            if(index == 0) return nullptr;
            index--;
        } else if((direction == RIGHT && orientation == horizontal) or (direction == DOWN && orientation == vertical)) {
            index++;
            if(index > subWins.size()) return nullptr;
        } else {
            return nullptr;
        }
    }
}
