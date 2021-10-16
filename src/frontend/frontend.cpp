#include "implementUI.h"

AbstractFrontEnd *AbstractFrontEnd::factory() {
    return new ImplFrontEnd();
}

std::list<uptr<Card>>
AbstractFrontEnd::chooseCardsToKeep(std::list<std::unique_ptr<Card>> &list, unsigned int nbToDiscard) {
    return getBasicIO()->checklist(list, nbToDiscard, nbToDiscard);
}
