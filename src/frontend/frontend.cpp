#include "implementUI.h"
#include "gameplay/2cards.h" //there seems to be an implicit deletion of an uptr<Card> in chooseCardsToKeep() ?

AbstractFrontEnd* AbstractFrontEnd::factory() {
    return new ImplFrontEnd();
}

std::list<CardWrapper>
AbstractFrontEnd::chooseCardsToKeep(std::list<CardWrapper>& list, unsigned int nbToDiscard) {
    return getBasicIO()->checklist(list, nbToDiscard, nbToDiscard);
}
