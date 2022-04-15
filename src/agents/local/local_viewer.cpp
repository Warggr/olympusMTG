#include "localagent.h"
#include "control/3player.h" //for player to Gamer conversion

void LocalViewer::onDraw(const std::list<CardWrapper>& cards) {
    frontEnd->addCards(cards);
}

void LocalViewer::showTop(const std::forward_list<card_ptr>& cards, uint nb) {
    auto iter = cards.begin();
    std::advance(iter, nb);
    frontEnd->disp(cards.begin(), iter);
}

void LocalViewer::registerMe(Gamer* pl) {
    frontEnd->registerMe(pl);
}
