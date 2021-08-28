#ifndef OLYMPUS_RULESHOLDER_H
#define OLYMPUS_RULESHOLDER_H

class PermOption; class Trigger_H; class StaticAb_H;
class ReaderVisitor; class TriggerHolder_H;

#include "8options.h"

struct CardOptionListNode {
    CardOption option;
    CardOptionListNode* next {nullptr};

    CardOption* operator->() { return &option; }
};

struct RulesHolder {
    SpellOption cast;
    CardOptionListNode* otherCardOptions;
    uint nb_actabs;
    PermOption* first_actab;
    uint nb_triggers;
    TriggerHolder_H* triggers;
    uint nb_statics;
    StaticAb_H* statics;
    char* flavor_text; //First, that will be P/T or starting loyalty. Else that's everything we haven't implemented yet.
    //Static abilities go into that category until we implement them.
    //In the end, it will only be flavor text, once everything will have reAchEd BlEsSed PeRFEcTIoN...
    //BeHOLD OUR GREAT WORK... ALL WILL BE ALL WILL ALL WILL BE ONE LONG LIVE PHYREXIA WSHHSHHHYRXXXHHYWWWYXRIA
};

#endif //OLYMPUS_RULESHOLDER_H
