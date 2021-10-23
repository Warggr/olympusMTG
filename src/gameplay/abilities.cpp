#include "gameplay/resolvables/5resolvables.h"
#include "oracles/classes/1effects.h"
#include "control/3player.h"

void Player::addMana(char c){
	manapool += c;
}

void Damageable::damage(int nb_damage, Target* origin){
	life -= nb_damage;
	is_damaged.trigger(getDmgController(), getMeAsTarget());
	origin->deals_damage.trigger(origin->getController(), nullptr);
}

void Effect_H::activate(SpecificTargeter<Target> *list_of_targets, Player *ctrl, Target *origin) const {
    for(auto& atom : effects) {
        atom.activate(list_of_targets, ctrl, origin);
    }
}

void AtomEffect_H::activate(Targeter* list_of_targets, Player* ctrl, Target* origin) const {
    using namespace effect;
	Target* target1;
    if(params[0] == 0){
        target1 = dynamic_cast<Target*> (ctrl);
    } else {
        target1 = list_of_targets[params[0]-1].getTarget();
    }
	switch(type){
		case deal_damage:
			(dynamic_cast<Damageable*> (target1))->damage(params[1], origin); break;
		case draw:
			(dynamic_cast<Player*> (target1))->draw(params[1]); break;
		case gain_life:
			(dynamic_cast<Player*> (target1))->gainLife(params[1]); break;
		case set_life:
			(dynamic_cast<Player*> (target1))->setLife(params[1]); break;
		case add_mana:
			(dynamic_cast<Player*> (target1))->addMana(params[1]); break;
		case destroy:
			(dynamic_cast<Permanent*> (target1))->destroy(); break;
	    case exile:
            (dynamic_cast<Permanent*>(target1))->exile(); break;
		case counter:
			(dynamic_cast<Resolvable*> (target1))->counter(); break;
	    case untap:
            (dynamic_cast<Permanent*>(target1))->untap(); break;
        default:
			gdebug(DBG_IMPORTANT | DBG_INTERNAL) << "Internal error: this ability (" << static_cast<int>(type) <<") hasn't been implemented yet\n";
    }
}

//To understand the use of the Target* origin, a quick example: Niv-Mizzet, the Firemind. His ability "ping 1" goes on the stack.
//When the ability resolves, if Niv's still alive, you will draw a card. If he's not, you won't.
//mostly, origin will have been drawn from a Targeter
