#include ".header_link.h"
#include "../HFiles/9modifs.h"

void Player::add_mana(char c){
	manapool += c;
}

void Player::damage(int nb_damage, Target* origin){
	life -= nb_damage;
	is_damaged.trigger(this, this);
}

void Creature::damage(int nb_damage, Target* origin){
	life -= nb_damage;
	is_damaged.trigger(ctrl, this);
}

void Planeswalker::damage(int nb_damage, Target* origin){
	life -= nb_damage;
	is_damaged.trigger(ctrl, this);
}

void Ability::activate(Targeter* list_of_targets, Player* ctrl, Target* origin) const {
	Target* target1;
	if(param1 == 0){
		target1 = dynamic_cast<Target*> (ctrl);
	}
	else {
		target1 = list_of_targets[(int) param1-1].getTarget();
	}
	switch(type){
		case 0:
			(dynamic_cast<Damageable*> (target1))->damage(param2, origin); break; //TODO: trigger the origin's damaging triggers, if that origin still exists
		case 1:
			(dynamic_cast<Player*> (target1))->draw(param2); break;
		case 2:
			(dynamic_cast<Player*> (target1))->gain_life(param2); break;
		case 3:
			(dynamic_cast<Player*> (target1))->set_life(param2); break;
		case 4:
			(dynamic_cast<Player*> (target1))->add_mana(param2); break;
		case 5:
			(dynamic_cast<Permanent*> (target1))->destroy(); break;
	    case 6:
            (dynamic_cast<Permanent*>(target1))->exile(); break;
		case 7:
			(dynamic_cast<Resolvable*> (target1))->counter(god.game); break;
		case 8:
			(dynamic_cast<Creature*> (target1))->plus_power(param2); break;
		case 9:
			(dynamic_cast<Creature*> (target1))->plus_toughness(param2); break;
	    case 11:
            (dynamic_cast<Permanent*>(target1))->untap(); break;
        default:
			god.gdebug(DBG_IMPORTANT | DBG_INTERNAL) << "Internal error: this ability ("<< type <<") hasn't been implemented yet\n";
	}
	if(next) next->activate(list_of_targets, ctrl, origin);
}

//To understand the use of the Target* origin, a quick example: Niv-Mizzet, the Firemind. His ability "ping 1" goes on the stack.
//When the ability resolves, if Niv's still alive, you will draw a card. If he's not, you won't.
//mostly, origin will have been drawn from a Targeter