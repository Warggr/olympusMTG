#include "implementUI.h"
#include "gameplay/2cards.h"

AbstractFrontEnd* AbstractFrontEnd::factory() {
    return new ImplFrontEnd();
}

std::list<CardWrapper> AbstractFrontEnd::chooseCardsToKeep(std::list<CardWrapper>& list, unsigned int nbToDiscard) {
    return getBasicIO()->checklist(list, nbToDiscard, nbToDiscard);
}
