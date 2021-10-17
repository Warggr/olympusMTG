#include "resolvables/5resolvables.h"
#include "permanents/4permanents.h"
#include "headC_constants.h"
#include "oracles/classes/2triggers.h"
#include "3player_cards.h"
#include "2cards.h"


const std::string Resolvable::description = "this spell or ability";

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
