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

bool CardWrapper::isCastable(const CastingContext& context) const {
    if(origin->getType().land)
        return context.sorcerySpeed and context.player->canPlayLand();
    else return (context.sorcerySpeed or origin->hasFlash()) and (context.player->manapool >= origin->getCost().mana);
}

const Option* CardWrapper::chooseOptionAction(const CastingContext&) const {
    return this;
}

bool PermOption::isCastable(const CastingContext& context) const {
    //TODO FEATURE some abilities are only sorcery-speed
    return context.player->manapool >= content.cost.mana;
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

bool CardOption::isCastable(const CastingContext& context) const {
    return Option::isCastable(context);
}

bool CardOption::castOpt(Player* ) { return true; /*TODO*/ }

bool Player::pay(Cost cost) {
    if(not (manapool >= cost.mana)) return false;
    manapool -= cost.mana;
    return true;
}

Player* Gamer::getDmgController() { return dynamic_cast<Player*>(this); }
Player* Gamer::getController() { return dynamic_cast<Player*>(this); }
