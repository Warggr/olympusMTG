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
