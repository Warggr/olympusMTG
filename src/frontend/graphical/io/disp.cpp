#include "gameplay/resolvables/5resolvables.h"
#include "gameplay/permanents/4permanents.h"
#include "gameplay/2cards.h"
#include "lib3_IO.h"
#include "oracles/classes/2triggers.h"
#include "oracles/classes/8options.h"
#include "oracles/classes/perm_option.h"

thread_local AbstractIO* AbstractIO::god = nullptr;

/*void Resolvable::disp(const Rect& zone, bool highlight) const {
	ImplementIO::god->draw_boxed_text(describe(), AbstractIO::BLACK, highlight ? AbstractIO::HIGH2 : AbstractIO::WHITE, zone.y, zone.z, zone.width);
}

void Effect_H::disp(const Rect& zone, const std::string origin_name) const { //mimicks a highlighted Resolvable which's on top of the stack
	ImplementIO::god->draw_boxed_text(origin_name, AbstractIO::BLACK, AbstractIO::HIGH1, zone.y, zone.z, zone.width);
}

void SpellOption::disp(int y, int z, int width, bool highlight, bool castable) const {
	std::string tmp = source->describe();
	ImplementIO::god->draw_boxed_text(tmp, highlight ? AbstractIO::HIGH2 : AbstractIO::WHITE, AbstractIO::BLACK, y, z, width);
	if(highlight) source->poster();
}*/

void AbstractIO::disp(const Permanent& perm, const Rect& zone, bool highlight) {
	god->draw_permanent(zone, perm.name, main_color(perm.color), !perm.untapped, highlight, false);
	if(highlight) poster(*perm.source->oracle);
}

void AbstractIO::disp(const Creature& crea, const Rect& zone, bool highlight) {
	disp((Permanent&) crea, zone, highlight);
	char pt[6]; sprintf(pt, "%d/%d", crea.getPower(), crea.getToughness());
	god->print_text(pt, AbstractIO::BLACK, zone.y + zone.width/2, zone.z + zone.height - 20);
}

void AbstractIO::disp(const Card& card, const Rect &pos, bool highlight) {
    disp(*(card.oracle), pos, highlight);
}

void AbstractIO::disp(const EmptyOption* opt, const Rect &pos, bool highlight) {
    (void) opt; (void) pos; (void) highlight; //TODO
}

void AbstractIO::disp(const Resolvable& res, const Rect &pos, bool highlight) {
    draw_boxed_text(res.describe(), highlight ? WHITE : BLACK, highlight ? HIGH1 : GREY, pos.y, pos.z, pos.width);
}