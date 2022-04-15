#ifndef OLYMPUS_2_CARDS_H
#define OLYMPUS_2_CARDS_H

#include "5rulesholder.h"
#include "displayable.h"
#include "serializable.h"
#include "headE_enums.h"
#include "Mana/lib2_mana.h"
#include <string>
#include <vector>

using Identifier = int;
class TriggerEvent;

/** represents an Oracle text. Should never be modified; will mostly be handled via a const shared pointer in a Card. */
class CardOracle : public Displayable {
public:
    typedef card_type type_t;
protected:
    std::string name;
    type_t type;
    colorId::type color;
    RulesHolder rules;
public:
    void init(ReaderVisitor& reader);

    std::string describe() const override;
    void disp(BasicIO* io) const override;
    std::vector<std::string> allText(int& power, int& toughness, int& frametype) const;

    inline Cost getCost() const { return rules.cost; }
    inline const std::string& getName() const { return name; }
    inline type_t getType() const { return type; }
    void getTriggers(trig_type type, TriggerEvent& trigEv) const;

    friend class Card;
    friend class AbstractIO;
    template<bool read> friend void visit(ConstHost<CardOracle, read>&, Visitor<read>&);
};

#endif //OLYMPUS_2_CARDS_H
