#include "gameplay/resolvables/5resolvables.h"
#include "gameplay/permanents/4permanents.h"
#include "gameplay/2cards.h"
#include "lib3_IO.h"
#include "oracles/classes/2triggers.h"
#include "oracles/classes/8options.h"
#include "oracles/classes/PermOption.h"

ImplementIO* AbstractIO::god = nullptr;

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
	char pt[6]; sprintf(pt, "%d/%d", crea.get_power(), crea.get_toughness());
	god->print_text(pt, AbstractIO::BLACK, zone.y + zone.width/2, zone.z + zone.height - 20);
}

void AbstractIO::disp(const std::unique_ptr<Card> &card, const Rect &pos, bool highlight) {
    disp(*(card->oracle), pos, highlight);
}

void AbstractIO::disp(const Option* opt, const Rect &pos, bool highlight) {
    //TODO
}

void AbstractIO::disp(const CardOracle &card, const Rect &pos, bool highlight) {
	const char* nametypes[] = {"Instant", "Land", "Artifact", "Planeswalker", "Creature", "Sorcery"};
	std::vector<std::string> all_text;
	/*if(card.rules.otherCardOptions) all_text.push_back(card.rules.otherCardOptions->describe(name));
	for(int i=0; i<nb_actabs; i++){
		all_text.push_back(first_actab[i].describe(name));
	}
	for(int i=0; i<nb_triggers; i++){
		all_text.push_back(triggers[i].describe((int) typeof_triggers[i], name));
	}*/
	int power = 0, toughness = 0, frametype = 0, offset = 0;
	if(card.type.underlying == card_type::creature){
		offset = 2;
		frametype = 1;
		//power = flavor_text[0];
		//toughness = flavor_text[1];
	}
	else if(card.type.underlying == card_type::planeswalker){
		offset = 1;
		frametype = 2;
		//power = flavor_text[0];
	}
	//if(flavor_text) if(flavor_text[offset]) all_text.push_back(std::string(flavor_text + offset));*/

	god->poster(card.name, card.getCost(), main_color(card.color),
                card.type.toString().c_str(), all_text, power, toughness, frametype, card.type.land);
	//lands have watermarks
}
