#include "../Yggdrasil/headB_board.h"

bool Player::chooseattackers(){
	if(myboard.mycreas.empty()) return false;
	myboard.myattackers = myboard.mycreas.def_new_state();
	bool ret = god.myUI->chooseattackers(myboard.mycreas, *(myboard.myattackers), player_id);
	disp_zone(3);
	disp_zone(4);
	return ret;
}

void Player::chooseblockers(StatedCollectionTN<Creature>& attackers, UIElement* attackerDisplay){
	god.myUI->chooseblockers(myboard.mycreas, attackers, permUI[3], attackerDisplay);
}

void Creature::resolve_attack(Player* nextopponent){
	if(!is_block){
		hit((Damageable*) nextopponent);
	}
	else if(assigned_bl.size() == 1){
		Creature* blocker = assigned_bl.front();
		hit(blocker);
		blocker->hit(this);
	}
	else{
		//int i = 0;
		for(auto itb = assigned_bl.begin(); itb != assigned_bl.end(); itb++){
			//god.myIO->message(&("How many damage to: " + (*itb)->describe())[0]);
			//int a = 5; //mvwscanw(winzones[6],1,0, "%d", &a); //the = 5 is there as long as IO does not give us a "getInt" function
			//if(a + i > iter->get_power()) a = iter->get_power();
			hit(*itb); //(*itb)->damage(a);
			(*itb)->hit(this);
			//i += a;
		}
	}
	assigned_bl.clear();
}

void Player::damagestep(){
	SET_TIME_FLAGS(0x80);
	if(myboard.myattackers == 0) return;
	for(auto & myattacker : *myboard.myattackers){
		myattacker.resolve_attack(nextopponent);
	}
	myboard.myattackers = nullptr;
	myboard.mycreas.restate(); //destroying attackers list
	god.game->statebasedactions();
}

void Creature::add_blocker(Creature* bl){
	is_block = true;
	assigned_bl.push_front(bl);
}