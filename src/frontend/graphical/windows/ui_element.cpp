#include "window.h"

DirectioL UIElement::direction = NOT_RECOGNIZED;

AbstractIO* UIElement::io = nullptr;

void UIElement::eraseBackground() const {
    io->erase_surface(coords);
}
