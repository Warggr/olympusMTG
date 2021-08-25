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

std::string TriggerEvent::describe(int typeoftrig, const std::string& cardname) const {
	std::string ds;
	for(auto all_trigger : all_triggers){
		ds += all_trigger->describe(typeoftrig, cardname);
	}
	return ds;
}

void Card::reveal() const {
    //TODO
}

std::string card_type::toString() const {
    std::string ret;
    if(legendary) ret += "Legendary";
    if(underlying == basic) ret += "Basic";
    if(snow) ret += "Snow";
    if(artifact) ret += "Artifact";
    if(enchantment) ret += "Enchantment";
    if(land) ret += "Land";
    switch(underlying) {
        case flagged:
        case basic:
            return ret;
        case creature:
            return ret + "Creature";
        case planeswalker:
            return ret + "Planeswalker";
        case instant:
            return ret + "Instant";
        case sorcery:
            return ret + "Sorcery";
    }
}
