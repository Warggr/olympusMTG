#include "visitor.h"
#include "classes/1effects.h"
#include "classes/2triggers.h"
#include "classes/3statics.h"
#include "classes/4actabs.h"

void RulesHolder::get_read(ReaderVisitor &visitor, card_type type) {
    visitor.readAll(*this, type);
}

void StaticAb_H::init(ReaderVisitor& ifile){
    ifile.readSelector(chars, requs); //reads up to ':'
    ifile.readModifier(nb_effects, first_effect, other_effects); //reads from ' ...' to '}' or '.'
}

void Effect_H::init(ReaderVisitor& visitor) {
    visitor.readEffectH(nb_parameters, parameters, effects);
}

void ActAb_H::init(ReaderVisitor &visitor) {
    visitor.readActAb(cost, effects, tapsymbol, ismanaability);
}

void AtomEffect_H::init(ReaderVisitor &reader, char* allassignedparams, uint8_t& nbassignedparams) {
    reader.readAtomEffect(type, params, nbassignedparams, allassignedparams);
}

void TriggerHolder_H::init(ReaderVisitor &visitor) {
    visitor.readTriggerType(type);
    trigger.init(visitor);
}

void Trigger_H::init(ReaderVisitor& visitor) {
    visitor.read_section_onresolve(effects);
}

void CardOracle::get_read(ReaderVisitor &reader) {
    reader.readName(name);
    reader.readManaCost(cost);
    reader.readCardType(type);
    color = cost.m2color(); if(type.land) color = 0; //lands are colorless

    reader.readAll(rules, type);
}