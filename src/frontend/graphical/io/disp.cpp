#include "gameplay/resolvables/5resolvables.h"
#include "gameplay/permanents/4permanents.h"
#include "gameplay/2cards.h"
#include "lib3_IO.h"
#include "oracles/classes/2triggers.h"
#include "oracles/classes/8options.h"
#include "oracles/classes/perm_option.h"

thread_local AbstractIO* AbstractIO::god = nullptr;

void AbstractIO::disp(const Permanent& perm, const Rect& zone, bool highlight) {
    god->draw_permanent(zone, perm.name, main_color(perm.color), !perm.untapped, highlight, false);
    if(highlight) poster(*perm.source->oracle);
}

void AbstractIO::disp(const Creature& crea, const Rect& zone, bool highlight) {
    disp((Permanent&) crea, zone, highlight);
    char pt[6]; sprintf(pt, "%d/%d", crea.getPower(), crea.getToughness());
    god->print_text(pt, AbstractIO::BLACK, zone.y + zone.width/2, zone.z + zone.height - 20);
}

void AbstractIO::disp(const Card& card, const Rect& pos, bool highlight) {
    disp(*(card.oracle), pos, highlight);
}

void AbstractIO::disp(const EmptyOption* opt, const Rect& pos, bool highlight) {
    (void) opt; (void) pos; (void) highlight; //TODO
}

void AbstractIO::disp(const Resolvable& res, const Rect& pos, bool highlight) {
    draw_boxed_text(res.describe(), highlight ? WHITE : BLACK, highlight ? HIGH1 : GREY, pos.y, pos.z, pos.width);
}
