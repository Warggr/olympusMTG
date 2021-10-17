#include "oracles/classes/8options.h"
#include "oracles/classes/perm_option.h"
#include "2cards.h"
#include "control/3player.h"
#include <iostream>
#include "resolvables/5resolvables.h"
#include "resolvables/stack.h"

//These functions are somehow ignored by the linker. So they have been defined again in createopts.cpp
bool SpellOption::iscastable(const Player* pl) const {
    if(isLand) return pl->canPlayLand();
    else return pl->manapool >= cost;
}

void SpellOption::cast_opt(Player* pl, uptr<Card>& origin){
    gdebug(DBG_TARGETING) << "CASTING SPELL " << origin->getName() << "\n";
    EmptyOption::payCosts(pl);
    if(isLand) pl->resolvePlayland(std::move(origin));
    else Stack::god->addToStack(std::make_unique<Spell>(std::move(origin), pl));
}

bool PermOption::iscastable(const Player* pl) const {
    return pl->manapool >= cost;
}

void EmptyOption::payCosts(Player *pl) const {
    pl->manapool -= cost;
}

void PermOption::straight_cast(Player* pl){
    Resolvable typecasted(pl, &effects);
    typecasted.resolve();
}

void PermOption::cast_opt(Player* pl, Permanent& origin){
    EmptyOption::payCosts(pl);
    if(tapsymbol) origin.tap();
    Stack::god->addToStack(std::make_unique<Resolvable>(pl, &effects));
}
