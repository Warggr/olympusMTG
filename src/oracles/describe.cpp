#include "headC_constants.h"
#include "oracles/classes/1effects.h"
#include "oracles/classes/card_oracle.h"
#include "oracles/classes/2triggers.h"
#include "oracles/classes/3statics.h"
#include "oracles/classes/perm_option.h"
#include <vector>

std::string PermOption::describe(const std::string& cardname) const {
    std::string ret = "[" + cost.m2t() + "]";
    if(tapsymbol){
        ret += ", [T]";
    }
    ret += ": ";
    ret += effects.describe(cardname);
    return ret;
}

std::string CardOracle::describe() const {
    std::string ret = name;
    if(type.land == 0){
        ret += "  [" + getCost().m2t() + "]";
    }
    return ret;
}

std::string Effect_H::describe(const std::string& name) const {
    std::string ret;
    for(const auto& atom : effects) ret += atom.describe(name);
    return ret;
}

std::string TriggerHolder_H::describe(const std::string &name, const Effect_H *effects, trigtype type) {
    std::string ds = trigger_descriptions[type];
    std::string ret = "Whenever ";
    for(char d : ds){
        if(d == '~') ret += name;
        else ret += d;
    }
    ret += ". ";
    ret += effects->describe(name);
    return ret;
}

std::string TriggerHolder_H::describe(const std::string& name) const {
    return describe(name, effects, type);
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

std::vector<std::string> CardOracle::allText(int& power, int& toughness, int& frametype) const {
    std::vector<std::string> all_text;
    for(const auto& i : rules.otherCardOptions) {
        all_text.push_back(i.describe(name));
    }
    for(uint i=0; i<rules.nb_actabs; i++){
        all_text.push_back(rules.first_actab[i].describe(name));
    }
    for(uint i=0; i<rules.nb_triggers; i++){
        all_text.push_back(rules.triggers[i].describe(name));
    }
    int offset = 0;
    if(type.underlying == card_type::creature){
        offset = 2;
        frametype = 1;
        power = rules.flavor_text[0];
        toughness = rules.flavor_text[1];
    }
    else if(type.underlying == card_type::planeswalker){
        offset = 1;
        frametype = 2;
        power = rules.flavor_text[0];
    }
    if(rules.flavor_text && rules.flavor_text[offset]) all_text.emplace_back(rules.flavor_text + offset);
    return all_text;
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
