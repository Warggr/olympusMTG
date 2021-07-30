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
    Mana cost;
    type_t type; //0 Instant, 1 Land, 2 ArtiEnch, 3 PW, 4 Creature, 5 Sorcery
    char color; //5 bits for 5 colors
    RulesHolder rules;

    Identifier casted_id;
public:
    CardOracle(ReaderVisitor& reader) { get_read(reader); }
    void get_read(ReaderVisitor& reader);
    void get_written(WriterVisitor& writer) const;

    void get_triggers(char type, TriggerEvent& trigEv) const;
    Identifier generate_casted_id() const;
    std::string describe() const;

    friend class Card;
};

#endif //OLYMPUS_2_CARDS_H
