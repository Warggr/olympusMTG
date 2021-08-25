#ifndef OLYMPUS_2_CARDS_H
#define OLYMPUS_2_CARDS_H

#include <string>
#include <fstream>
#include "headE_enums.h"
#include "Mana/lib2_mana.h"
#include "oracles/classes/5rulesholder.h"

using Identifier = int;
class TriggerEvent;
class ReaderVisitor; class WriterVisitor;

class CardOracle{ //represents an Oracle text. Should never be modified.
public:
    typedef card_type type_t;
protected:
    std::string name;
    type_t type;
    colorId::type color;
    RulesHolder rules;

//    Identifier casted_id;
public:
    explicit CardOracle(ReaderVisitor& reader) { get_read(reader); }
    void get_read(ReaderVisitor& reader);

    void get_triggers(char type, TriggerEvent& trigEv) const;
//    Identifier generate_casted_id() const;
    std::string describe() const;
    inline Mana getCost() const { return rules.cast.cost; }

    friend class Card;
    friend class AbstractIO;
};

#endif //OLYMPUS_2_CARDS_H
