#include "../HFiles/classes.h"

void Ability::activate(Targeter* list_of_targets, Player* ctrl){
	Target* target1;
	if(param1 == 0){
		target1 = dynamic_cast<Target*> (ctrl);	
	}
	else {
		target1 = list_of_targets[(int) param1-1].getTarget();
	}
	switch(type){
		case 0:{
			(dynamic_cast<Damageable*> (target1))->damage(param2);
			} break;
		case 1:
			(dynamic_cast<Player*> (target1))->draw(param2);
			break;
		case 2:
			(dynamic_cast<Player*> (target1))->gain_life(param2);
			break;
		case 3:
			(dynamic_cast<Player*> (target1))->set_life(param2);
			break;
		case 4:
			(dynamic_cast<Player*> (target1))->add_mana(param2);
			break;
		case 5:
			(dynamic_cast<Permanent*> (target1))->destroy(false);
			break;
		case 6:
			(dynamic_cast<Resolvable*> (target1))->counter(ctrl->metagame);
			break;
		default:
			god.gdebug(DBG_IMPORTANT | DBG_INTERNAL) << "Internal error: this ability ("<< type <<") hasn't been implemented yet\n";
	}
	if(next) next->activate(list_of_targets, ctrl);
}