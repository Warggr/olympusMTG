#ifndef OLYMPUS_CLASSES_CARDS_2_H
#define OLYMPUS_CLASSES_CARDS_2_H

#include "1general.h"
#include "optionwrappers.h"
#include "Mana/cost.h"
#include "classes/card_oracle.h"
#include <memory>
#include <forward_list>

#ifdef ORACLE_ARRAY
#define card_ptr const CardOracle*
#define move_cardptr(x) x
#define new_cardptr new CardOracle
#else
#define card_ptr std::shared_ptr<const CardOracle>
#define move_cardptr(x) std::move(x)
#define new_cardptr std::make_shared<const CardOracle>
#endif

/**Represents a physical card.
 * Class Card is as minimal as possible in order not to take too much space when packed 60 times in a deck.
 * In particular, it doesn't remember its zone or its controller.
 *
 * Therefore, Cards aren't Options (an Option needs to know where it is in order to clean itself up). See #class CardWrapper.*/
class Card: public Displayable {
public:
    enum zone { library, graveyard, exile, command };
	card_ptr oracle;

	explicit Card(card_ptr orc): oracle(move_cardptr(orc)) { }

    void reveal() const;
    std::string describe() const override { return oracle->describe(); };

	card_type getType() const { return oracle->getType(); };
	bool hasFlash() const { return oracle->type.underlying == card_type::sorcery and oracle->type.shift; } //TODO implement flash
	Cost getCost() const { return oracle->getCost(); };
	const Effect_H* getEffect() const { return oracle->rules.effects; };
	colorId::type getColor() const { return oracle->color; };
	void getPermabs(PermOption** pr, int* nb_opts) const { *pr = oracle->rules.first_actab; *nb_opts = oracle->rules.nb_actabs; };
	void getTriggers(trig_type type, TriggerEvent& trigEv) const { oracle->getTriggers(type, trigEv); };
	const char* getFlavorText() const { return oracle->rules.flavor_text; }
	std::string getName() const { return oracle->getName(); }

	void write(WriterVisitor& writer) const;
	void disp(BasicIO* io) const override;

	friend class CardWrapper;
};

/** A CardWrapper contains a card and information about its zone and owner. */
class CardWrapper: public OptionWrapper, public Option, public Target {
    uptr<Card> origin;
    Player* owner;
public:
    CardWrapper(uptr<Card> origin, Player* owner):
        Target(origin->getName()), origin(std::move(origin)), owner(owner) { t_type = target_type::card; };
    Card& operator*() { return *origin; }
    Card* get() { return origin.get(); }
    const Card* get() const { return origin.get(); }
    Card* operator->() { return origin.get(); }
    const Card* operator->() const { return origin.get(); }
    uptr<Card> unwrap() { return std::move(origin); }

    void disp(BasicIO* io) const override { origin->disp(io); }
    std::string describe() const override { return origin->describe(); };
    Option* chooseOptionAction() override;
    Player* getController() override { return owner; }

    void castOpt(Player* pl) override;
    bool isCastable(bool sorceryspeed, Player* player) const override;

    bool operator==(const CardWrapper& other) const { return this == &other; }
};

#endif //OLYMPUS_CLASSES_CARDS_2_H
