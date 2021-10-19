#include "oracles/classes/8options.h"
#include "oracles/classes/perm_option.h"
#include "2cards.h"
#include "control/3player.h"
#include <iostream>
#include "resolvables/5resolvables.h"
#include "resolvables/stack.h"

void DefaultCardOption::castOpt(Player* pl){
    gdebug(DBG_TARGETING) << "CASTING SPELL " << origin->getName() << "\n";
    if(origin->getType().land) {
        pl->resolvePlayland(std::move(origin));
    } else {
        pl->manapool -= origin->getCost();
        Stack::god->addToStack(std::make_unique<Spell>(std::move(origin), pl));
    }
}

bool DefaultCardOption::isCastable(bool sorceryspeed) const {
    return (sorceryspeed or origin->hasFlash()) and (origin->getController()->manapool >= origin->getCost());
}

bool PermOption::isCastable(bool sorceryspeed) const {
    (void) sorceryspeed; //TODO some abilities are only sorcery-speed
    return origin->getController()->manapool >= cost;
}

void PermOption::straight_cast(Player* pl){
    Resolvable typecasted(pl, &effects);
    typecasted.resolve();
}

void PermOption::castOpt(Player* pl){
    pl->manapool -= cost;
    if(tapsymbol) origin->tap();
    Stack::god->addToStack(std::make_unique<Resolvable>(pl, &effects));
}

std::string CardOption::describe(const std::string& cardName) const {
    (void) cardName;
    return std::string(); //TODO
}
