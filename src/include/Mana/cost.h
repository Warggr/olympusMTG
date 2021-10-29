#ifndef OLYMPUS_COST_H
#define OLYMPUS_COST_H

#include "lib2_mana.h"

struct Cost {
    Mana mana;
    //WeirdCost* otherCosts;
    Cost() = default;
    Cost(Mana mana): mana(mana) {};
};

#endif //OLYMPUS_COST_H
