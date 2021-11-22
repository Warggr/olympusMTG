#include "classes/8options.h"
#include "classes/perm_option.h"
#include "2cards.h"
#include "control/3player.h"
#include <iostream>
#include "resolvables/5resolvables.h"
#include "resolvables/stack.h"

void CardWrapper::castOpt(Player* pl){
    gdebug(DBG_TARGETING) << "CASTING SPELL " << origin->getName() << "\n";
    if(origin->getType().land) {
        pl->resolvePlayland(std::move(origin));
    } else {
        pl->pay(origin->getCost());
        Stack::god->addToStack(std::make_unique<Spell>(std::move(origin), pl));
    }
    pl->getHand().remove(*this);
}

bool CardWrapper::isCastable(bool sorceryspeed, Player* player) const {
    return (sorceryspeed or origin->hasFlash()) and (player->manapool >= origin->getCost().mana);
}

Option* CardWrapper::chooseOptionAction() {
    return this;
}

bool PermOption::isCastable(bool sorceryspeed, Player* player) const {
    (void) sorceryspeed; //TODO some abilities are only sorcery-speed
    return player->manapool >= cost.mana;
}

void PermOption::straight_cast(Player* pl){
    Resolvable typecasted(pl, &effects);
    typecasted.resolve();
}

void PermOption::castOpt(Player* pl){
    pl->manapool -= cost.mana;
    if(tapsymbol) origin->tap();
    Stack::god->addToStack(std::make_unique<Resolvable>(pl, &effects));
}

bool CardOption::isCastable(bool sorceryspeed, Player* player) const {
    return Option::isCastable(sorceryspeed, player);
}

void CardOption::castOpt(Player* ) { /*TODO*/ }

void CardOption::disp(BasicIO*) const { /*TODO*/ }

bool Player::pay(Cost cost) {
    if(not (manapool >= cost.mana)) return false;
    manapool -= cost.mana;
    return true;
}
