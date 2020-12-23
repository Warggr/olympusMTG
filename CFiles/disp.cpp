#include ".header_link.h"
#include "../HFiles/8options.h"
#include "../HFiles/10triggers.h"

void Resolvable::disp(int y, int z, int width, int height, bool highlight) const {
	std::string text = describe();
	god.myIO->draw_full_rectangle(highlight ? Abstract_io::HIGH2 : Abstract_io::WHITE, y, z, width, height);
	god.myIO->print_text(&(text[0]), Abstract_io::BLACK, y, z);
}

void PreResolvable::disp(int y, int z, int width, int height, std::string origin_name) const { //mimicks a highlighted Resolvable which's on top of the stack
	std::string text = describe(origin_name);
	god.myIO->draw_full_rectangle(Abstract_io::HIGH1, y, z, width, height);
	god.myIO->print_text(&(text[0]), Abstract_io::BLACK, y, z);
}

void SpellOption::disp(int y, int z, bool highlight, bool castable) const {
	//if(castable) wattron(win, A_BOLD);
	std::string tmp = source->describe();
	char clr = highlight ? Abstract_io::HIGH2 : Abstract_io::WHITE; //red or white
	god.myIO->draw_full_rectangle(Abstract_io::BLACK, y, z, 400, 25);
	god.myIO->print_text(&(tmp[0]), clr, y, z);
	if(highlight) source->poster();
}

void Permanent::disp(int left, int top, int width, int height, bool highlight) const {
	god.myIO->draw_permanent(left, top, width, height, color, !((bool)(flags&1)), highlight, false);
	if(highlight) source->poster();
	god.myIO->print_text(name, 0, left, top);
}

void Land::disp(int left, int top, int width, int height, bool highlight) const {
	god.myIO->draw_permanent(left, top, width, height, color, !((bool)(flags&1)), highlight, true);
	if(highlight) source->poster();
	god.myIO->print_text(name, 0, left, top);
}

void Creature::disp(int left, int top, int width, int height, bool highlight) const {
	Permanent::disp(left, top, width, height, highlight);
	char pt[6]; sprintf(pt, "%d/%d", get_power(), get_toughness());
	god.myIO->print_text(pt, Abstract_io::BLACK, left + width/2, top + height - 20);
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

	god.myIO->poster(name, cost, color, nametypes[(int) type], all_text, power, toughness, frametype, (type == 1));
	//lands have watermarks
}
