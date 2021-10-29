#include "localagent.h"

void LocalViewer::onDraw(const std::list<CardWrapper>& cards) {
    frontEnd->addCards(cards);
}

void LocalViewer::showTop(const std::forward_list<std::unique_ptr<Card>> &cards, uint nb) {
    auto iter = cards.begin();
    for(uint i = 0; i < nb; ++iter, ++i);
    frontEnd->disp(cards.begin(), iter);
}

void LocalViewer::registerMe(Player* pl) {
    frontEnd->registerMe(pl);
}
