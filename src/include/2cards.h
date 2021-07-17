#ifndef OLYMPUS_CLASSES_CARDS_2_H
#define OLYMPUS_CLASSES_CARDS_2_H

#include "1general.h"
#include "../Mana/lib2_mana.h"
#include <memory>
#include <forward_list>

class CardOracle{ //represents an Oracle text. Should never be modified.
	std::string name;
	Mana cost;
	char type; //0 Instant, 1 Land, 2 ArtiEnch, 3 PW, 4 Creature, 5 Sorcery
	char color; //5 bits for 5 colors
	PreResolvable* on_cast; //fun fact: all Ancestral Recalls have a pointer to the same 'draw three cards' ability. I hope that won't cause bugs, but I can't see how
	int nb_actabs;
	PermOption* first_actab; //as PW (usually) don't have actabs, they will use that field. They'll mask a '+1' effect as an effect that costs [1] and a '-3' effect as one that costs [WWWW].
	int nb_triggers;
	char* typeof_triggers;
	Trigger* triggers;
	int nb_statics;
	StaticAb* statics;
	char* flavor_text; //First, that will be P/T or starting loyalty. Else that's everything we haven't implemented yet.
	//Static abilities go into that category until we implement them.
	//In the end, it will only be flavor text, once everything will have reAchEd BlEsSed PeRFEcTIoN...
	//BeHOLD OUR GREAT WORK... ALL WILL BE ALL WILL ALL WILL BE ONE LONG LIVE PHYREXIA WSHHSHHHYRXXXHHYWWWYXRIA
    Identifier casted_id;
public:
	CardOracle(std::ifstream& bFile);
	CardOracle(std::ifstream& myfile, std::string n, Mana c, char t);

	std::string describe() const;
	void poster() const;

	void get_triggers(char type, TriggerEvent& trigEv) const;
	void write_binary(std::ofstream& bFile) const;
	Identifier generate_casted_id() const;

	friend class Card;
};

class Card: public Target{
	std::shared_ptr<const CardOracle> oracle;
public:
	Card(const std::shared_ptr<const CardOracle>& orc): Target(&(orc->name)), oracle(orc) {t_type = 0x02; };
	std::string describe() const {return oracle->describe(); };
	void poster() const {oracle->poster(); };

	char get_type() const {return oracle->type; };
	bool has_flash() const {return oracle->type == 0; } //TODO implement flash
	Mana get_cost() const {return oracle->cost; };
	const PreResolvable* get_preRes() const {return oracle->on_cast; };
	char get_color() const {return oracle->color; };
	void get_permabs(PermOption** pr, int* nb_opts) const { *pr = oracle->first_actab; *nb_opts = oracle->nb_actabs; };
	void get_triggers(const char type, TriggerEvent& trigEv) const {oracle->get_triggers(type, trigEv); };
	const char* get_flavor_text() const {return oracle->flavor_text; }
    Identifier casted_id() const {return oracle->casted_id; };
	Identifier reload_id() const;
	//get_name is provided by being a child of Target
};

#endif //OLYMPUS_CLASSES_CARDS_3_H