#include ".header_link.h"

bool Player::chooseattackers(){
	if(mycreas.empty()) return false;
	bool ret = god.myUI->chooseattackers(mycreas, myattackers, player_id);
	disp_zone(3);
	disp_zone(4);
	return ret;
}

void Player::chooseblockers(std::list<Creature>& attackers, UIElement* attackerDisplay){
	god.myUI->chooseblockers(mycreas, attackers, permUI[3], attackerDisplay);
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
		for(std::list<Creature*>::iterator itb = assigned_bl.begin(); itb != assigned_bl.end(); itb++){
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
	if(myattackers.empty()) return;
	for(auto iter = myattackers.begin(); iter != myattackers.end(); iter++){
		iter->resolve_attack(nextopponent);
	}
	mycreas.splice(mycreas.begin(), myattackers); //taking back fighters
	god.game->statebasedactions();
}

void Creature::add_blocker(Creature* bl){
	is_block = true;
	assigned_bl.push_front(bl);
}