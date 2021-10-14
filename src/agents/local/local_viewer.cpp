#include "localagent.h"

void LocalViewer::onDraw(const std::list<std::unique_ptr<Card>>& cards) {
    frontEnd.ui.addCards(cards);
}
