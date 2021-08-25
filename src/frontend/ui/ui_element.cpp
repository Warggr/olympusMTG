#include "12defaultUI.h"
#include "window.h"

#include <iostream>

DirectioL UIElement::direction = NOT_RECOGNIZED;

void UIElement::erase_background(AbstractIO *io) const {
    io->erase_surface(coords);
}

ListElement::ListElement(int top, int left, int width, int height, int offset, int nbItems, bool direction):
        GalleryElement(nbItems, direction, top, left, width, height), yOffset(0), zOffset(0) {
    if(direction == UIElement::vertical) zOffset = offset;
    else yOffset = offset;
    if(offset < 0) { //reversed zone
        coords.shift(yOffset, zOffset);
    }
    gdebug(DBG_IOUI) << "Dimensions: -" << left << "-" << left + nbItems * yOffset + width << "-"
              ",|" << top << "|" << top + nbItems * zOffset + height << "\n";
}

Rect ListElement::get_coordinates(int* yOff, int* zOff) const {
    *yOff = yOffset;
    *zOff = zOffset;
    return coords;
}

void ListElement::get_coordinates(int* y, int* z, int* yOff, int* zOff) const {
    *y = coords.y;
    *z = coords.z;
    *yOff = yOffset;
    *zOff = zOffset;
}

void ListElement::get_coordinates(int itemNb, int* y, int* z) const {
    *y = coords.y + itemNb * yOffset;
    *z = coords.z + itemNb * zOffset;
}

Target *ListElement::iterate(char requs, bool needstarget) {
    return nullptr; //TODO
}
