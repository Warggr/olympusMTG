#ifndef OLYMPUS_2_CARDS_H
#define OLYMPUS_2_CARDS_H

#include <string>
#include <fstream>
#include "Mana/lib2_mana.h"
#include "visitor.h"

using Identifier = int;
class Effect_H; class PermOption; class Trigger_H; class StaticAb_H; class TriggerEvent;

class card_type {
    enum basic_type { flagged, basic, creature, planeswalker, instant, sorcery };
    char legendary : 1, snow : 1;
    char land : 1, artifact : 1, enchantment : 1;
    basic_type underlying : 3;
};

class CardOracle{ //represents an Oracle text. Should never be modified.
public:
    typedef card_type type_t;
protected:
    std::string name;
    Mana cost;
    type_t type; //0 Instant, 1 Land, 2 ArtiEnch, 3 PW, 4 Creature, 5 Sorcery
    char color; //5 bits for 5 colors
    Effect_H* on_cast; //fun fact: all Ancestral Recalls have a pointer to the same 'draw three cards' ability. I hope that won't cause bugs, but I can't see how
    int nb_actabs;
    PermOption* first_actab; //as PW (usually) don't have actabs, they will use that field. They'll mask a '+1' effect as an effect that costs [1] and a '-3' effect as one that costs [WWWW].
    int nb_triggers;
    char* typeof_triggers;
    Trigger_H* triggers;
    int nb_statics;
    StaticAb_H* statics;
    char* flavor_text; //First, that will be P/T or starting loyalty. Else that's everything we haven't implemented yet.
    //Static abilities go into that category until we implement them.
    //In the end, it will only be flavor text, once everything will have reAchEd BlEsSed PeRFEcTIoN...
    //BeHOLD OUR GREAT WORK... ALL WILL BE ALL WILL ALL WILL BE ONE LONG LIVE PHYREXIA WSHHSHHHYRXXXHHYWWWYXRIA
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
