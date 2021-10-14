#ifndef OLYMPUS_2_CARDS_H
#define OLYMPUS_2_CARDS_H

#include "headE_enums.h"
#include "Mana/lib2_mana.h"
#include "oracles/classes/5rulesholder.h"
#include <string>
#include <fstream>
#include <vector>

using Identifier = int;
class TriggerEvent;
class ReaderVisitor;

class CardOracle{ //represents an Oracle text. Should never be modified.
public:
    typedef card_type type_t;
protected:
    std::string name;
    type_t type;
    colorId::type color;
    RulesHolder rules;
public:
#ifdef F_TESTS
    CardOracle(type_t type): name("Test card"), type(type) {}
#endif
    explicit CardOracle(ReaderVisitor& reader) { init(reader); }
    void init(ReaderVisitor& reader);

    std::string describe() const;
    std::vector<std::string> allText(int &power, int &toughness, int& frametype) const;

    inline Mana getCost() const { return rules.cast.cost; }
    inline const std::string& getName() const { return name; }
    inline type_t getType() const { return type; }
    void getTriggers(char type, TriggerEvent& trigEv) const;

    friend class Card;
    friend class AbstractIO;
};

#endif //OLYMPUS_2_CARDS_H
