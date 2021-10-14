#ifndef OLYMPUS_OPTIONMANAGER_H
#define OLYMPUS_OPTIONMANAGER_H

#include "gameplay/2cards.h"

class OptionManager {
    Player* player; //TODO optimize away
public:
    OptionManager(Player* pl): player(pl) {}
    bool hasOptions(bool sorcerySpeed) const;
};

#endif //OLYMPUS_OPTIONMANAGER_H
