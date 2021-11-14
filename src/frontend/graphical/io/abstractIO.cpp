#include "implementIO.h"
#include "6abstractIO.h"
#include "gameplay/2cards.h"

AbstractIO* AbstractIO::factory() {
    return new ImplementIO();
}

void AbstractIO::draw(const CardWrapper& card, const Rect& rect, bool highlight) {
    draw(*card->oracle, rect, highlight);
}
