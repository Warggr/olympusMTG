#ifndef OLYMPUS_RULESHOLDER_H
#define OLYMPUS_RULESHOLDER_H

#include "8options.h"
#include "perm_option.h"
#include "2triggers.h"
#include "3statics.h"
#include "Mana/cost.h"
#include <vector>
#include <forward_list>

class StaticAb_H;

struct RulesHolder {
    Cost cost;
    Effect_H* effects;
    std::forward_list<CardOption> otherCardOptions;
    std::vector<PermOption_H> actabs;
    std::vector<TriggerHolder_H> triggers;
    std::vector<StaticAb_H> statics;
    char* flavor_text {nullptr}; //First, that will be P/T or starting loyalty. Else that's everything we haven't implemented yet.
    //Static abilities go into that category until we implement them.
    //In the end, it will only be flavor text, once everything will have reAchEd BlEsSed PeRFEcTIoN...
    //BeHOLD OUR GREAT WORK... ALL WILL BE ALL WILL ALL WILL BE ONE LONG LIVE PHYREXIA WSHHSHHHYRXXXHHYWWWYXRIA

    ~RulesHolder();
};

#endif //OLYMPUS_RULESHOLDER_H
