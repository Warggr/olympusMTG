#include "headC_constants.h"
#include "oracles/classes/1effects.h"
#include "oracles/classes/2cards.h"
#include "oracles/classes/2triggers.h"
#include "oracles/classes/3statics.h"
#include "oracles/classes/PermOption.h"

std::string PermOption::describe(const std::string& cardname) const {
    std::string ret = "[" + cost.m2t() + "]";
    if(tapsymbol){
        ret += ", [T]";
    }
    ret += ": ";
    ret += effects->describe(cardname);
    return ret;
}

std::string CardOracle::describe() const {
    std::string ret = name;
    if(type.land == 0){
        ret += "  [" + getCost().m2t() + "]";
    }
    return ret;
}

std::string Trigger_H::describe(int typeoftrig, const std::string& name) const {
    std::string ds = trigger_descriptions[typeoftrig];
    std::string ret = "Whenever ";
    for(char d : ds){
        if(d == '~') ret += name;
        else ret += d;
    }
    ret += ". ";
    ret += effects->describe(name);
    return ret;
}

std::string AtomEffect_H::describe(const std::string& cardname) const {
    std::string ds = ability_descriptions[static_cast<int>(type)];
    std::string ret;
    for(char d : ds){
        if(d == '~') ret += cardname;
        else if(d == '?') ret += std::to_string(params[1]);
        else ret += d;
    }
    ret += ".";
    return ret;
}
