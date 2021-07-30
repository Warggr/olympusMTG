#include "4actabs.h"
#include "2cards.h"

std::string ActAb_H::describe(const std::string& cardname) const {
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
        ret += "  [" + cost.m2t() + "]";
    }
    return ret;
}
