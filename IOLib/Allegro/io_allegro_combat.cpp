#include "11allegroIO.h"

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
