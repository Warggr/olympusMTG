#ifndef OLYMPUS_RULESHOLDER_H
#define OLYMPUS_RULESHOLDER_H

class PermOption; class Trigger_H; class StaticAb_H;
class ReaderVisitor; struct TriggerHolder_H;

#include "8options.h"

struct CardOptionListNode {
    CardOption option;
    CardOptionListNode* next {nullptr};

    CardOption* operator->() { return &option; }
};

struct RulesHolder {
    SpellOption cast;
    CardOptionListNode* otherCardOptions {nullptr};
    uint nb_actabs {0};
    PermOption* first_actab {nullptr};
    uint nb_triggers {0};
    TriggerHolder_H* triggers {nullptr};
    uint nb_statics {0};
    StaticAb_H* statics {nullptr};
    char* flavor_text {nullptr}; //First, that will be P/T or starting loyalty. Else that's everything we haven't implemented yet.
    //Static abilities go into that category until we implement them.
    //In the end, it will only be flavor text, once everything will have reAchEd BlEsSed PeRFEcTIoN...
    //BeHOLD OUR GREAT WORK... ALL WILL BE ALL WILL ALL WILL BE ONE LONG LIVE PHYREXIA WSHHSHHHYRXXXHHYWWWYXRIA

    ~RulesHolder();
};

#endif //OLYMPUS_RULESHOLDER_H
