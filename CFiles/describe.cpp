#include "../HFiles/olympus_main.h"

std::string PreResolvable::describe(std::string name) const {
	return fab->describe(name);
}

void CardZone::describe(char* tmp) const {
	//mvwprintw(w, 1, y, "%s (%d)", name, size);
}

std::string CardOracle::describe() const {
	std::string ret = name;
	if(type != 1){
		ret += "  [" + cost.m2t() + "]";
	}
	return ret;
}

std::string PermOption::describe(std::string cardname) const {
	std::string ret = "[" + cost.m2t() + "]";
	if(tapsymbol){
		ret += ", [T]";
	}
	ret += ": ";
	ret += effects->describe(cardname);
	return ret;
}

std::string Permanent::describe() const {
	return source->describe();
}

std::string Creature::describe() const {
	return source->describe();
}

std::string TriggerEvent::describe(int typeoftrig, std::string cardname) const {
	std::string ds;
	for(auto iter = all_triggers.begin(); iter != all_triggers.end(); iter++){
		ds += (*iter)->describe(typeoftrig, cardname);
	}
	return ds;
}

std::string Trigger::describe(int typeoftrig, std::string name) const {
	std::string ds = olympus::trigger_descriptions[typeoftrig];
	std::string ret = "Whenever ";
	for(unsigned int i=0; i<ds.length(); i++){
		if(ds[i] == '~') ret += name;
		else ret += ds[i];
	}
	ret += ". ";
	ret += effects->describe(name);
	return ret;
}

std::string Ability::describe(std::string cardname) const {
	std::string ds = olympus::abilities_descriptions[(int) type];
	std::string ret;
	for(unsigned int i=0; i<ds.length(); i++){
		if(ds[i] == '~') ret += cardname;
		else if(ds[i] == '?') ret += std::to_string(param2);
		else ret += ds[i];
	}
	ret += ".";
	if(next){
		ret += " " + next->describe(cardname);
	}
	return ret;
}