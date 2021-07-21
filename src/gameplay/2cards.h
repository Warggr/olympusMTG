#ifndef OLYMPUS_CLASSES_CARDS_2_H
#define OLYMPUS_CLASSES_CARDS_2_H

#include "gameplay/1general.h"
#include "Mana/lib2_mana.h"
#include <memory>
#include <forward_list>

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

#endif //OLYMPUS_CLASSES_CARDS_2_H