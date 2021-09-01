#ifndef OLYMPUS_DICTHOLDER_H
#define OLYMPUS_DICTHOLDER_H

#include "oracles/classes/3statics.h"
#include "lib1_dict.h"
#include "headE_enums.h"

enum selectors { types, subtypes, tribes, players };

struct DictHolder {
    const Dictionary_tpl<trigtype> dict_trigtypes;
    const Dictionary_tpl<effect_type> dict_abiltypes;
    const Dictionary_tpl<Modifier::type> dict_static_types;
    const Dictionary_tpl<selectors> dict_selectors;
    const Dictionary dict_selector_types;
    const Dictionary dict_selector_subtypes;
    const Dictionary dict_selector_tribes;
    const Dictionary dict_selector_players;

    DictHolder();
};

#endif //OLYMPUS_DICTHOLDER_H
