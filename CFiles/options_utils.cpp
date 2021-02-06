#include "../HFiles/8options.h"
#include "head1_utilities.h"
#include "../HFiles/12abilities.h"

void PermOption::write_binary(std::ofstream& bFile) const {
    set_canary('e', bFile);
    effects->write_binary(bFile);
    char twobools = (tapsymbol ? 0x10 : 0x00) + (ismanaability ? 0x01 : 0x00);
    bFile.write(&twobools, sizeof(char));
    set_canary('f', bFile);
}

void PermOption::read_binary(std::ifstream& bFile){
    check_canary('e', bFile);
    effects = new PreResolvable(bFile);
    char twobools;
    bFile.read(&twobools, sizeof(char));
    tapsymbol = (bool) (twobools / 0x10);
    ismanaability = (bool) (twobools % 0x10);
    check_canary('f', bFile);
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