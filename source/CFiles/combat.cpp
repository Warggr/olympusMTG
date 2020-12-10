#include "../HFiles/olympus_main.h"

bool Player::chooseattackers(){
	if(mycreas.empty()) return false;
	bool ret = god.myUI->chooseattackers(mycreas, myattackers, player_id);
	disp_zone(3);
	disp_zone(4);
	return ret;
}

bool Default_ui::chooseattackers(std::list<Creature>& cowards, std::list<Creature>& warriors, char player_id){
	myIO->message("Choose Attackers");
	bool ret = false;
	int i = 0;
	int y, z, yOff, zOff, ySize, zSize;
	permanentZones[(player_id-1) * 5 + 3]->get_coordinates(&y, &z, &yOff, &zOff, &ySize, &zSize);
	for(std::list<Creature>::iterator iter = cowards.begin(); iter != cowards.end(); i++){
		if((iter->get_flags()&3) == 3){ //untapped and no summoning sickness
			int zPos = (player_id == 1) ? z + i*zOff + zSize : z + i*zOff; //getting either top or bottom Z
			bool attacks = myIO->attack_switch(y + i*yOff, ySize, zPos, (player_id == 1) ? 20 : -20);
			if(attacks){
				std::list<Creature>::iterator newatt = iter;
				newatt->disp(y+i*yOff, z+i*zOff, ySize, zSize, false); //disp creature normally
				newatt->reset_flags(1); //tap creature
				iter++;
				warriors.splice(warriors.begin(), cowards, newatt); //move creature to list "warriors"
				ret = true;
			}
			else{
				iter->disp(y+i*yOff, z+i*zOff, ySize, zSize, false); //disp creature normally
				iter++;
			}
		}
		else iter++;
	}
	return ret;
}

void Player::chooseblockers(std::list<Creature>& attackers, UIElement* attackerDisplay){
	god.myUI->chooseblockers(mycreas, attackers, permUI[3], attackerDisplay);
}

void Default_ui::chooseblockers(std::list<Creature>& defenders, std::list<Creature>& attackers, UIElement* defenderDisplay, UIElement* attackerDisplay){
	int pos = 0;
	for(std::list<Creature>::iterator blocker = defenders.begin(); blocker != defenders.end(); blocker++){
		if(blocker->get_flags()&1){ //untapped
			int y, z;
			defenderDisplay->get_coordinates(pos, &y, &z);
			Creature* evilguy = myIO->blocker_switch(*blocker, y, z, attackers, attackerDisplay, permanentYSize, permanentZSize);
			if(evilguy){
				blocker->set_blocking();
				evilguy->add_blocker(&(*blocker));
			}
		}
		pos++;
	}
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
	metagame->statebasedactions();
}

bool Allegro_io::attack_switch(int leftY, int Ywidth, int topZ, int arrowlength){
	bool attacks = false;
	while(1){
		ALLEGRO_EVENT event;
		al_flip_display();
		al_wait_for_event(queue, &event);
		switch(event.type){
			case ALLEGRO_EVENT_KEY_CHAR:
				switch(event.keyboard.keycode){
					case ALLEGRO_KEY_UP:
						attacks = true;
						al_draw_filled_triangle(leftY, topZ, leftY+Ywidth, topZ, leftY + (Ywidth/2), topZ + arrowlength, registeredColors[HIGH1]);
						message("[ATTACKS]");
						//al_flip_display(); //already done by messaging
						break;
					case ALLEGRO_KEY_DOWN:
						attacks = false;
						erase_surface(leftY, topZ, Ywidth, arrowlength);
						message("[STAYS]");
						//al_flip_display(); //already done by messaging
						break;
					case ALLEGRO_KEY_RIGHT:
					case ALLEGRO_KEY_ENTER:
					case ALLEGRO_KEY_SPACE:
						return attacks;
				}
				break;
			case ALLEGRO_EVENT_DISPLAY_CLOSE:
				god.minimalKill();
				exit(1);
		}
	}
}

Creature* Allegro_io::blocker_switch(const Creature& blocker, int blockerY, int blockerZ, std::list<Creature>& attackers, UIElement* attacker_io, int creatureWidth, int creatureDepth){
	int pos_evilguy = 0;
	blocker.disp(blockerY, blockerZ, creatureWidth, creatureDepth, true);
	std::list<Creature>::iterator evilguy = attackers.end();
	while(1){ //getting creature to block
		al_flip_display();
		ALLEGRO_EVENT event;
		al_wait_for_event(queue, &event);
		if(event.type==ALLEGRO_EVENT_KEY_CHAR){
			if(evilguy != attackers.end()){
				int y, z;
				attacker_io->get_coordinates(pos_evilguy, &y, &z);
				evilguy->disp(y, z, creatureWidth, creatureDepth, false); //displaying villain normally
				if(event.keyboard.keycode == ALLEGRO_KEY_DOWN) evilguy = attackers.end();
				if(event.keyboard.keycode == ALLEGRO_KEY_LEFT && evilguy != attackers.begin()){
					pos_evilguy--;
					evilguy--;
				}
				if(event.keyboard.keycode == ALLEGRO_KEY_RIGHT){
					evilguy++;
					if(evilguy == attackers.end()) evilguy--;
					else pos_evilguy++;
				}
			}
			else{
				if(event.keyboard.keycode == ALLEGRO_KEY_UP){
					evilguy = attackers.begin();
					pos_evilguy = 0;
					blocker.disp(blockerY, blockerZ, creatureWidth, creatureDepth, false); //displaying blocker normally
				}
			}
			if(event.keyboard.keycode == ALLEGRO_KEY_SPACE || event.keyboard.keycode == ALLEGRO_KEY_ENTER){
				if(evilguy != attackers.end()) return &(*evilguy);
				else return 0;
			}
			if(evilguy != attackers.end()){
				int y, z;
				attacker_io->get_coordinates(pos_evilguy, &y, &z);
				evilguy->disp(y, z, creatureWidth, creatureDepth, true); //displaying villain
			}
			else{
				blocker.disp(blockerY, blockerZ, creatureWidth, creatureDepth, true);
			}
		}
		else if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
			god.minimalKill();
			exit(1);
		}
	}
}

void Creature::add_blocker(Creature* bl){
	is_block = true;
	assigned_bl.push_front(bl);
}