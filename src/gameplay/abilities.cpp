#include "oracles/classes/1effects.h"
#include "control/3player.h"

void Player::add_mana(char c){
	manapool += c;
}

void Damageable::damage(int nb_damage, Target* origin){
	life -= nb_damage;
	is_damaged.trigger(getController(), (Target*)(this));
}

void AtomEffect_H::activate(Targeter* list_of_targets, Player* ctrl, Target* origin) const {
	Target* target1;
	if(params[0] == 0){
		target1 = dynamic_cast<Target*> (ctrl);
	} else {
		target1 = list_of_targets[(int) param1-1].getTarget();
	}
	switch(type){
		case deal_damage:
			(dynamic_cast<Damageable*> (target1))->damage(param2, origin); break; //TODO: trigger the origin's damaging triggers, if that origin still exists
		case draw:
			(dynamic_cast<Player*> (target1))->draw(param2); break;
		case gain_life:
			(dynamic_cast<Player*> (target1))->gain_life(param2); break;
		case set_life:
			(dynamic_cast<Player*> (target1))->set_life(param2); break;
		case add_mana:
			(dynamic_cast<Player*> (target1))->add_mana(param2); break;
		case destroy:
			(dynamic_cast<Permanent*> (target1))->destroy(); break;
	    case exile:
            (dynamic_cast<Permanent*>(target1))->exile(); break;
		case counter:
			(dynamic_cast<Resolvable*> (target1))->counter(god.game); break;
		case plus_power:
			(dynamic_cast<Creature*> (target1))->plus_power(param2); break;
		case plus_toughness:
			(dynamic_cast<Creature*> (target1))->plus_toughness(param2); break;
	    case untap:
            (dynamic_cast<Permanent*>(target1))->untap(); break;
        default:
			gdebug(DBG_IMPORTANT | DBG_INTERNAL) << "Internal error: this ability ("<< type <<") hasn't been implemented yet\n";
	}
}

//To understand the use of the Target* origin, a quick example: Niv-Mizzet, the Firemind. His ability "ping 1" goes on the stack.
//When the ability resolves, if Niv's still alive, you will draw a card. If he's not, you won't.
//mostly, origin will have been drawn from a Targeter
