#include <utility>

#include ".header_link.h"
#include "../HFiles/12abilities.h"
#include "../HFiles/8options.h"
#include "oracles/10triggers.h"

void Resolvable::disp(const Rect& zone, bool highlight) const {
	god.myIO->draw_boxed_text(describe(), Abstract_io::BLACK, highlight ? Abstract_io::HIGH2 : Abstract_io::WHITE, zone.y, zone.z, zone.width);
}

void PreResolvable::disp(const Rect& zone, const std::string origin_name) const { //mimicks a highlighted Resolvable which's on top of the stack
	god.myIO->draw_boxed_text(origin_name, Abstract_io::BLACK, Abstract_io::HIGH1, zone.y, zone.z, zone.width);
}

void SpellOption::disp(int y, int z, int width, bool highlight, bool castable) const {
	std::string tmp = source->describe();
	god.myIO->draw_boxed_text(tmp, highlight ? Abstract_io::HIGH2 : Abstract_io::WHITE, Abstract_io::BLACK, y, z, width);
	if(highlight) source->poster();
}

void Permanent::disp(const Rect& zone, bool highlight) const {
	god.myIO->draw_permanent(zone, *name, main_color(color), !((bool)(flags&1)), highlight, false);
	if(highlight) source->poster();
}

void Land::disp(const Rect& zone, bool highlight) const {
	god.myIO->draw_permanent(zone, *name, main_color(color), !((bool)(flags&1)), highlight, true);
	if(highlight) source->poster();
}

void Creature::disp(const Rect& zone, bool highlight) const {
	Permanent::disp(zone, highlight);
	char pt[6]; sprintf(pt, "%d/%d", get_power(), get_toughness());
	god.myIO->print_text(pt, Abstract_io::BLACK, zone.y + zone.width/2, zone.z + zone.height - 20);
}

void CardOracle::poster() const {
	const char* nametypes[] = {"Instant", "Land", "Artifact", "Planeswalker", "Creature", "Sorcery"};
	std::vector<std::string> all_text;
	if(on_cast) all_text.push_back(on_cast->describe(name));
	for(int i=0; i<nb_actabs; i++){
		all_text.push_back(first_actab[i].describe(name));
	}
	for(int i=0; i<nb_triggers; i++){
		all_text.push_back(triggers[i].describe((int) typeof_triggers[i], name));
	}
	int power = 0, toughness = 0, frametype = 0, offset = 0;
	if(type == 4){
		offset = 2;
		frametype = 1;
		power = flavor_text[0];
		toughness = flavor_text[1];
	}
	else if(type == 3){
		offset = 1;
		frametype = 2;
		power = flavor_text[0];
	}
	if(flavor_text) if(flavor_text[offset]) all_text.push_back(std::string(flavor_text + offset));

	god.myIO->poster(name, cost, main_color(color), nametypes[(int) type], all_text, power, toughness, frametype, (type == 1));
	//lands have watermarks
}
