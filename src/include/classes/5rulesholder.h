#ifndef OLYMPUS_RULESHOLDER_H
#define OLYMPUS_RULESHOLDER_H

class PermOption; class StaticAb_H; struct TriggerHolder_H; class Effect_H;

#include "8options.h"
#include "Mana/cost.h"
#include <forward_list>

struct RulesHolder {
    Cost cost;
    Effect_H* effects;
    std::forward_list<CardOption> otherCardOptions;
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
