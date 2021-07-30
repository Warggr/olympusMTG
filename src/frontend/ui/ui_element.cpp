#include "12defaultUI.h"
#include <iostream>

UIElement::UIElement(int left, int top, int _ySize, int _zSize, int offset, int _maxItems, bool direction):
        topZ(top), leftY(left), yOffset(0), zOffset(0), ySize(_ySize), zSize(_zSize), maxItems(_maxItems) {
    if(direction == vertical) zOffset = offset;
    else yOffset = offset;
    if(offset < 0){ //reversed zone
        topZ += zOffset;
        leftY += yOffset;
    }
    gdebug(DBG_IOUI) << "Dimensions: -" << left <<"-"<< left + maxItems*yOffset + ySize <<"-,|"<< top <<"|"<< top + maxItems*zOffset + zSize << std::endl;
}

Rect UIElement::get_coordinates(int* yOff, int* zOff) const {
    *yOff = yOffset;
    *zOff = zOffset;
    return Rect(leftY, topZ, ySize, zSize);
}

void UIElement::get_coordinates(int* y, int* z, int* yOff, int* zOff) const {
    *y = leftY;
    *z = topZ;
    *yOff = yOffset;
    *zOff = zOffset;
}

void UIElement::get_coordinates(int itemNb, int* y, int* z) const {
    *y = leftY + itemNb * yOffset;
    *z = topZ + itemNb * zOffset;
}

void UIElement::erase_background(AbstractIO* io) const {
    io->erase_surface(Rect(leftY, topZ, (maxItems-1)*yOffset + ySize, (maxItems-1)*zOffset + zSize));
}
