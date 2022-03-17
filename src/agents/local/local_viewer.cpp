#include "localagent.h"
#include "control/3player.h" //for player to Gamer conversion

void LocalViewer::onDraw(const std::list<CardWrapper>& cards) {
    frontEnd->addCards(cards);
}

void LocalViewer::showTop(const std::forward_list<std::unique_ptr<Card>>& cards, uint nb) {
    auto iter = cards.begin();
    std::advance(iter, nb);
    frontEnd->disp(cards.begin(), iter);
}

void LocalViewer::registerMe(Player* pl) {
    frontEnd->registerMe(pl);
}
