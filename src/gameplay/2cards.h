#ifndef OLYMPUS_CLASSES_CARDS_2_H
#define OLYMPUS_CLASSES_CARDS_2_H

#include "1general.h"
#include "optionwrappers.h"
#include "Mana/lib2_mana.h"
#include "oracles/classes/card_oracle.h"
#include <memory>
#include <forward_list>

#ifdef ORACLE_ARRAY
#define card_ptr const CardOracle*
#define light_cardptr CardOracle*
#define new_cardptr new CardOracle
#else
#define card_ptr std::shared_ptr<const CardOracle>
#define light_cardptr const card_ptr&
#define new_cardptr std::make_shared<const CardOracle>
#endif

class Card: public Target {
    Player* owner;
public:
	card_ptr oracle;

	explicit Card(light_cardptr orc): Target(orc->name), oracle(orc) { t_type = target_type::card; }

    uptr<Option> chooseOptionAction(uptr<Card>& me);

    void reveal() const;
    std::string describe() const { return oracle->describe(); };

	card_type getType() const { return oracle->getType(); };
	bool hasFlash() const { return oracle->type.underlying == card_type::sorcery and oracle->type.shift; } //TODO implement flash
	Mana getCost() const { return oracle->getCost(); };
	const Effect_H* getEffect() const { return oracle->rules.cast.effects; };
	colorId::type getColor() const { return oracle->color; };
	void getPermabs(PermOption** pr, int* nb_opts) const { *pr = oracle->rules.first_actab; *nb_opts = oracle->rules.nb_actabs; };
	void getTriggers(trig_type type, TriggerEvent& trigEv) const { oracle->getTriggers(type, trigEv); };
	const char* getFlavorText() const {return oracle->rules.flavor_text; }
    Player* getController() override { return owner; }
	//getName is provided by being a child of Target

	void init(ReaderVisitor& visitor) const {
	    const_cast<CardOracle*>(oracle.get())->init(visitor);
	}
	void disp(BasicIO* io) const override;
};

#endif //OLYMPUS_CLASSES_CARDS_2_H
