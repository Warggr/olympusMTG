
#include <oracles/classes/card_oracle.h>

#include "visitor.h"
#include "oracles/classes/1effects.h"
#include "oracles/classes/2triggers.h"
#include "oracles/classes/3statics.h"
#include "oracles/classes/perm_option.h"

void StaticAb_H::init(ReaderVisitor& ifile){
    ifile.readSelector(chars, requs); //reads up to ':'
    ifile.readModifier(nb_effects, first_effect, other_effects); //reads from ' ...' to '}' or '.'
}

void Effect_H::init(ReaderVisitor& visitor) {
    visitor.readEffectH(nb_parameters, parameters, effects);
}

void PermOption::init(ReaderVisitor &visitor) {
    visitor.readActAb(cost, &effects, tapsymbol, ismanaability, instantspeed);
}

void AtomEffect_H::init(ReaderVisitor &reader, char* allassignedparams, uint8_t& nbassignedparams) {
    reader.readAtomEffect(type, params, nbassignedparams, allassignedparams);
}

void TriggerHolder_H::init(ReaderVisitor &visitor) {
    visitor.readTriggerType(type);
    effects->init(visitor);
}

void CardOracle::init(ReaderVisitor &reader) {
    reader.readName(name);
    reader.readCardType(type);

    if(type.land) {
        color = 0; //lands are colorless
    } else {
        reader.readCost(rules.cost);
        color = rules.cost.mana.m2color();
    }

    //reader.readCardSubtypes(); //TODO implement subtypes
    reader.readAll(rules, type);
}

namespace target_type {
    const flag_t tars1[] = { damageable, nonnegative };
    const flag_t tars2[] = { player, nonnegative };
    const flag_t tars3[] = { player, number };
    const flag_t tars4[] = { player, added_mana };
    const flag_t tars5[] = { permanent };
    const flag_t tars6[] = { resolvable };
    const flag_t tars7[] = { permanent, strictpositive };
    const flag_t tars8[] = { permanent };
    const flag_t* target_types[] = { tars1, tars2, tars3, tars3, tars4, tars5, tars5, tars6, tars7, tars8 };
    const int target_numbers[] = { 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1 };
}
