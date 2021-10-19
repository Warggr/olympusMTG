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
    visitor.readActAb(cost, additional_costs, &effects, tapsymbol, ismanaability, instantspeed);
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
        reader.readManaCost(rules.cast.cost);
        color = rules.cast.cost.m2color();
    }

    //reader.readCardSubtypes(); //TODO implement subtypes
    reader.readAll(rules, type);
//    casted_id = generate_casted_id();
}

RulesHolder::~RulesHolder() {
    delete[] first_actab;
    delete[] triggers;
    delete[] statics;
    delete[] flavor_text;
}
