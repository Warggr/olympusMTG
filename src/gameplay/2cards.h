#ifndef OLYMPUS_CLASSES_CARDS_2_H
#define OLYMPUS_CLASSES_CARDS_2_H

#include "1general.h"
#include "optionwrappers.h"
#include "Mana/cost.h"
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

class Card {
    Player* owner;
public:
	card_ptr oracle;

	explicit Card(light_cardptr orc): oracle(orc) { }

    void reveal() const;
    std::string describe() const { return oracle->describe(); };

	card_type getType() const { return oracle->getType(); };
	bool hasFlash() const { return oracle->type.underlying == card_type::sorcery and oracle->type.shift; } //TODO implement flash
	Cost getCost() const { return oracle->getCost(); };
	const Effect_H* getEffect() const { return oracle->rules.effects; };
	colorId::type getColor() const { return oracle->color; };
	void getPermabs(PermOption** pr, int* nb_opts) const { *pr = oracle->rules.first_actab; *nb_opts = oracle->rules.nb_actabs; };
	void getTriggers(trig_type type, TriggerEvent& trigEv) const { oracle->getTriggers(type, trigEv); };
	const char* getFlavorText() const { return oracle->rules.flavor_text; }
	std::string getName() { return oracle->getName(); }

	void init(ReaderVisitor& visitor) const {
	    const_cast<CardOracle*>(oracle.get())->init(visitor);
	}
	void disp(BasicIO* io) const;

	friend class CardWrapper;
};

//Cards aren't Options themselves; an Option must know where it is in order to clean itself up,
// while a Card does not remember in which zone it is. Cards are rather handled to a CardWrapper, which knows the exact
// location of the card's uptr.
class CardWrapper: public OptionWrapper, public Option, public Target {
    uptr<Card> origin;
public:
    CardWrapper(uptr<Card> origin): Target(origin->oracle->getName()), origin(std::move(origin)) { t_type = target_type::card; };
    Card& operator*() { return *origin; }
    Card* get() { return origin.get(); }
    const Card* get() const { return origin.get(); }
    Card* operator->() { return origin.get(); }
    const Card* operator->() const { return origin.get(); }
    uptr<Card> unwrap() { return std::move(origin); }
    void disp(BasicIO* io) const override { origin->disp(io); }
    std::string describe() const override { return origin->describe(); };
    Option* chooseOptionAction() override;
    Player* getController() override { return origin->owner; }

    void castOpt(Player* pl) override;
    bool isCastable(bool sorceryspeed, Player* player) const override;
};

#endif //OLYMPUS_CLASSES_CARDS_2_H
