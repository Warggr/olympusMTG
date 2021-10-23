#include "resolvables/5resolvables.h"
#include "permanents/4permanents.h"
#include "headE_enums.h"
#include "oracles/classes/2triggers.h"
#include "control/3player.h"
#include "2cards.h"
#include <boost/format.hpp>

const std::string Resolvable::description = "this spell or ability";

std::string Permanent::describe() const {
	return source->describe();
}

std::string Creature::describe() const {
	return source->describe();
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

std::string Player::describe() const {
    return (boost::format("{} ({} life)") % name % life).str();
}
