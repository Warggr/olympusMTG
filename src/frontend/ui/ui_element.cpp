#include "12defaultUI.h"
#include "window.h"

DirectioL UIElement::direction = NOT_RECOGNIZED;

void UIElement::erase_background(AbstractIO *io) const {
    io->erase_surface(coords);
}
