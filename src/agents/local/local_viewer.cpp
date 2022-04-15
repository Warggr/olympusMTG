#include "localagent.h"
#include "control/3player.h" //for player to Gamer conversion

void LocalViewer::onDraw(const std::list<CardWrapper>& cards) {
    frontEnd->addCards(cards);
}

void LocalViewer::registerMe(Gamer* pl) {
    frontEnd->registerMe(pl);
}
