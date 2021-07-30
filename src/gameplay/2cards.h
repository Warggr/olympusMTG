#ifndef OLYMPUS_CLASSES_CARDS_2_H
#define OLYMPUS_CLASSES_CARDS_2_H

#include "1general.h"
#include "Mana/lib2_mana.h"
#include "oracles/classes/2cards.h"
#include <memory>
#include <forward_list>

class SpellOption;

class Card: public Target, public OptionWrapper<SpellOption> {
	std::shared_ptr<const CardOracle> oracle;
public:
	explicit Card(const std::shared_ptr<const CardOracle>& orc): Target(orc->name), oracle(orc) { t_type = target_type::card; }
	std::string describe() const {return oracle->describe(); };
	void poster() const; //{ oracle->poster(); };

	card_type get_type() const { return oracle->type; };
	bool has_flash() const { return oracle->type.underlying == card_type::instant; } //TODO implement flash
	Mana get_cost() const { return oracle->cost; };
	const Effect_H* get_preRes() const { return oracle->rules.on_cast; };
	char get_color() const { return oracle->color; };
	void get_permabs(ActAb_H** pr, int* nb_opts) const { *pr = oracle->rules.first_actab; *nb_opts = oracle->rules.nb_actabs; };
	void get_triggers(const char type, TriggerEvent& trigEv) const { oracle->get_triggers(type, trigEv); };
	const char* get_flavor_text() const {return oracle->rules.flavor_text; }
    Identifier casted_id() const {return oracle->casted_id; };
	Identifier reload_id() const;
	//get_name is provided by being a child of Target
};

#endif //OLYMPUS_CLASSES_CARDS_2_H