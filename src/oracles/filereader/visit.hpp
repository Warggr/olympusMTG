#ifndef OLYMPUS_VISIT_HPP
#define OLYMPUS_VISIT_HPP

#include "classes/serializable.h"

class PermOption; class TriggerHolder_H; class CardOracle; class StaticAb_H;

template<bool read>
void visit(ConstHost<StaticAb_H, read>& host, Visitor<read>& visitor){
    visitor.readSelector(host.chars, host.requs); //reads up to ':'
    visitor.readModifier(host.nb_effects, host.first_effect, host.other_effects); //reads from ' ...' to '}' or '.'
}

template<bool read>
void visit(ConstHost<Effect_H, read>& effect, Visitor<read>& visitor) {
    visitor.readEffect(effect.effects, effect.nb_parameters, effect.parameters);
}

template<bool read>
void visit(ConstHost<PermOption, read>& h, Visitor<read>& visitor) {
    visitor.readActAb(h.cost, &h.effects, h.tapsymbol, h.ismanaability, h.instantspeed);
}

template<bool read>
void visit(ConstHost<TriggerHolder_H, read>& h, Visitor<read>& visitor) {
    visitor.visit("type", h.type);
    visit<read>(*h.effects, visitor);
}

template<bool read>
void visit(ConstHost<CardOracle, read>& h, Visitor<read>& visitor) {
    visitor.visit("name", h.name);
    visitor.visit("type", h.type);

    if(!h.type.land) {
        visitor.visit("cost", h.rules.cost);
    }

    visitor.readSubtypes(); //TODO implement subtypes
    visitor.readAll(h.rules, h.type);
}

template<bool read>
void visit(ConstHost<Cost, read>& cost, Visitor<read>& visitor) {
    visitor.visit("mana", cost.mana);
}

#endif //OLYMPUS_VISIT_HPP
