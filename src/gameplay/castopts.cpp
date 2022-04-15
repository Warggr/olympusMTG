#include "classes/8options.h"
#include "classes/perm_option.h"
#include "2cards.h"
#include "control/3player.h"
#include "resolvables/5resolvables.h"
#include "resolvables/stack.h"

bool CardWrapper::castOpt(Player* pl){
    gdebug(DBG_TARGETING) << "CASTING SPELL " << origin->getName() << "\n";
    if(origin->getType().land) {
        pl->resolvePlayland(std::move(origin));
    } else {
        if(!pl->pay(origin->getCost())) return false;
        Stack::god->addToStack(std::make_unique<Spell>(move_cardptr(origin), pl));
    }
    pl->getHand().remove(*this);
    return true;
}

bool CardWrapper::isCastable(bool sorceryspeed, const Player* player) const {
    if(origin->getType().land) return player->canPlayLand();
    else return (sorceryspeed or origin->hasFlash()) and (player->manapool >= origin->getCost().mana);
}

const Option* CardWrapper::chooseOptionAction() const {
    return this;
}

bool PermOption::isCastable(bool sorceryspeed, const Player* player) const {
    (void) sorceryspeed; //TODO FEATURE some abilities are only sorcery-speed
    return player->manapool >= content.cost.mana;
}

void PermOption::straight_cast(Player* pl){
    Resolvable typecasted(pl, &content.effects);
    typecasted.resolve();
}

bool PermOption::castOpt(Player* pl){
    if(!pl->pay(content.cost)) return false;
    if(content.tapsymbol){
        if(!origin->isUntapped()) return false;
        origin->tap();
    }
    Stack::god->addToStack(std::make_unique<Resolvable>(pl, &content.effects, origin));
    return true;
}

bool CardOption::isCastable(bool sorceryspeed, const Player* player) const {
    return Option::isCastable(sorceryspeed, player);
}

bool CardOption::castOpt(Player* ) { return true; /*TODO*/ }

bool Player::pay(Cost cost) {
    if(not (manapool >= cost.mana)) return false;
    manapool -= cost.mana;
    return true;
}

Player* Gamer::getDmgController() { return dynamic_cast<Player*>(this); }
Player* Gamer::getController() { return dynamic_cast<Player*>(this); }
