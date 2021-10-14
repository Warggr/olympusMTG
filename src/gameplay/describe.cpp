#include "resolvables/5resolvables.h"
#include "permanents/4permanents.h"
#include "headC_constants.h"
#include "oracles/classes/2triggers.h"
#include "3player_cards.h"

const std::string Resolvable::description = "this spell or ability";

std::string Effect_H::describe(const std::string& name) const {
    std::string ret;
	for(const auto& atom : effects) ret += atom.describe(name);
	return ret;
}

std::string Permanent::describe() const {
	return source->describe();
}

std::string Creature::describe() const {
	return source->describe();
}

std::string TriggerEvent::describe(trigtype type, const std::string& cardname) const {
	std::string ds;
	for(auto trigger : all_triggers){
		ds += TriggerHolder_H::describe(cardname, trigger, type);
	}
	return ds;
}

void Card::reveal() const {
    //TODO
}

std::string card_type::toString() const {
    std::string ret;
    if(legendary) ret += "Legendary ";
    if(snow) ret += "Snow ";
    if(artifact) ret += "Artifact ";
    if(enchantment) ret += "Enchantment ";
    if(land){
        if(shift) return ret + "Basic Land";
        else ret += "Land ";
    }
    switch(underlying) {
        case flagged:
            return ret;
        case creature:
            return ret + "Creature";
        case planeswalker:
            return ret + "Planeswalker";
        case sorcery:
            return ret + (shift ? "Instant" : "Sorcery");
    }
    return ret;
}
