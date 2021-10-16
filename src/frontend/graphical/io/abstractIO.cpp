#include "implementIO.h"
#include "6abstractIO.h"
#include "gameplay/2cards.h"

AbstractIO* AbstractIO::factory() {
    return new ImplementIO();
}

void AbstractIO::disp(const std::unique_ptr<Card>& card, const Rect& pos, bool highlight) {
    disp(*card->oracle, pos, highlight);
}

void AbstractIO::disp(const Card &card, const Rect &rect, bool highlight) {
    disp(*card.oracle, rect, highlight);
}
