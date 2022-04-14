#ifndef OLYMPUS_CLASSES_CARDS_2_H
#define OLYMPUS_CLASSES_CARDS_2_H

#include "1general.h"
#include "optionwrappers.h"
#include "Mana/cost.h"
#include "classes/card_oracle.h"
#include "head2_cardptrs.h"
#include <vector>
#include <memory>
#include <string>
#include <istream>
#include <forward_list>

/**Represents a physical card.
 * Class Card is as minimal as possible in order not to take too much space when packed 60 times in a deck.
 * In particular, it doesn't remember its zone or its controller.
 *
 * Therefore, Cards aren't Options (an Option needs to know where it is in order to clean itself up). See #class CardWrapper.*/
class Card: public Displayable {
public:
    enum zone { library, graveyard, exile, command };
    oracle_ptr oracle;

    Card() = default;
    explicit Card(oracle_ptr orc): oracle(move_oracleptr(orc)) { }

    void reveal() const;
    void disp(BasicIO* io) const override;
    std::string describe() const override { return oracle->describe(); };

    card_type getType() const { return oracle->getType(); };
    bool hasFlash() const { return oracle->type.underlying == card_type::sorcery and oracle->type.shift; } //TODO implement flash
    Cost getCost() const { return oracle->getCost(); };
    const Effect_H* getEffect() const { return oracle->rules.effects; };
    colorId::type getColor() const { return oracle->color; }
    const std::vector<PermOption_H>& getPermabs() const { return oracle->rules.actabs; }
    void getTriggers(trig_type type, TriggerEvent& trigEv) const { oracle->getTriggers(type, trigEv); };
    const char* getFlavorText() const { return oracle->rules.flavor_text; }
    const std::string& getName() const { return oracle->getName(); }

    std::string toStr(const CardOracle* offset) const;
    void fromStr(std::istream& ifile, const CardOracle* offset);
    void write(WriterVisitor& writer) const;

    friend class CardWrapper;
};

/** A CardWrapper contains a card and information about its zone and owner. */
class CardWrapper: public OptionWrapper, public Option, public Target {
    card_ptr origin;
    Player* owner;
public:
    CardWrapper(card_ptr origin, Player* owner):
        Target(origin->getName()), origin(move_cardptr(origin)), owner(owner) { t_type = target_type::card; };
    const Card& operator*() const { return *origin; }

    card_ptr get() const { return origin; }
    card_ptr operator->() const { return origin; }
    card_ptr unwrap() { return origin; }

    void disp(BasicIO* io) const override { origin->disp(io); }
    std::string describe() const override { return origin->describe(); };
    const Option* chooseOptionAction() const override;
    Player* getController() override { return owner; }

    bool castOpt(Player* pl) override;
    bool isCastable(bool sorceryspeed, const Player* player) const override;

    bool operator==(const CardWrapper& other) const { return this == &other; }
};

struct Deck {
    std::vector<Card> cards;
    std::vector<CardOracle> oracles;

    Deck() = default;
    Deck(Deck&& other): cards(std::move(other.cards)), oracles(std::move(other.oracles)) {};
};

#endif //OLYMPUS_CLASSES_CARDS_2_H
