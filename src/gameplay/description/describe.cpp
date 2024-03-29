#include "gameplay/resolvables/5resolvables.h"
#include "gameplay/permanents/4permanents.h"
#include "headE_enums.h"
#include "classes/2triggers.h"
#include "gameplay/gamer.h"
#include "gameplay/2cards.h"
#include <boost/format.hpp>

const std::string Resolvable::description = "this spell or ability";

std::string Permanent::describe() const {
    std::string ret;
    if(!isUntapped()) ret += "(T)";
    return ret + source->describe();
}

std::string Creature::describe() const {
    return Permanent::describe();
}

std::string TriggerEvent::describe(trig_type type, const std::string& cardname) const {
    std::string ds;
    for(auto trigger : all_triggers){
        ds += TriggerHolder_H::describe(cardname, trigger, type);
    }
    return ds;
}

void Card::reveal() const {
    //TODO
}

std::string Gamer::describe() const {
    return (boost::format("{} ({} life)") % name % life).str();
}

Permanent::type Permanent::getType() const {
    return source->getType().toPermType();
}

Mana Permanent::getManaCost() const { return source->getCost().mana; }
