#include "../HFiles/classes.h"

void Player::chooseattackers(){
	Permanent* coward = fperm[3]; //all creatures staying
	metagame->message("Choose Attackers");
	int pos = 0;
	while(1){
		bool attacks = false;
		while(1){ //FINDING THE COWARD
			if(!coward){
				disp_zone(3);
				disp_zone(4);
				return;
			}
			if((coward->get_flags()&3) == 3) break;
			coward = coward->next; pos++;
		}
		coward->disp(pos, 4, true);
		bool done = false;
		while(!done){ //MOVING THE COWARD
			ALLEGRO_EVENT event;
			al_flip_display();
			al_wait_for_event(god.queue, &event);
			switch(event.type){
				case ALLEGRO_EVENT_KEY_CHAR:
					switch(event.keyboard.keycode){
						case ALLEGRO_KEY_UP:
							attacks = true;
							al_draw_filled_rectangle(pos*CARD_Y + LEFTBAR_Y, HEADER_X + 4*CARD_X, (pos+1)*CARD_Y + LEFTBAR_Y, HEADER_X + 5*CARD_X, al_map_rgb(0, 0, 0));
							coward->disp(pos, 5, true);
							metagame->message("[COWARD]");
							break;
						case ALLEGRO_KEY_DOWN:
							attacks = false;
							coward->disp(pos, 4, true);
							metagame->message("[WARRIOR]");
							al_draw_filled_rectangle(pos*CARD_Y + LEFTBAR_Y, HEADER_X + 5*CARD_X, (pos+1)*CARD_Y + LEFTBAR_Y, HEADER_X + 6*CARD_X, al_map_rgb(0, 0, 0));
							break;
						case ALLEGRO_KEY_RIGHT:
						case ALLEGRO_KEY_ENTER:
						case ALLEGRO_KEY_SPACE:
							if(attacks){
								coward->disp(pos, 5, false); //disp creature normally
								Permanent* i2 = coward->next; //adding crea to attackers
								coward->pop();
								if(fperm[4]) fperm[4]->prev = coward;
								coward->next = fperm[4];
								coward->prev = 0;
								coward->reset_flags(1); //tap creature
								fperm[4] = coward;
								coward = i2;
							}
							else{
								coward->disp(pos, 4, false); //disp creature normally
								coward = coward->next;
							}
							pos++;
							attacks = false;
							done = true;
					}
					break;
				case ALLEGRO_EVENT_DISPLAY_CLOSE:
					god.minimalKill();
					exit(1);
			}
		}
		al_flip_display();
	}
}

void Player::chooseblockers(){
	if(!(nextopponent->fperm[4])) return;
	int pos = 0;
	for(Creature* blocker = (Creature*) fperm[3]; blocker!=0; blocker = blocker){
		if(blocker->get_flags()&1){
			int pos_evilguy = 0;
			Permanent* evilguy = 0;
			while(1){ //getting creature to block
				al_flip_display();
				ALLEGRO_EVENT event;
				al_wait_for_event(god.queue, &event);
				if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
					god.minimalKill();
					exit(1);
				}
				else if(event.type==ALLEGRO_EVENT_KEY_CHAR){
					if(evilguy){
						evilguy->disp(pos_evilguy, 5, false); //displaying villain normally
						if(event.keyboard.keycode == ALLEGRO_KEY_DOWN) evilguy = 0;
						if(event.keyboard.keycode == ALLEGRO_KEY_LEFT && evilguy->prev){
							evilguy = evilguy->prev;
							pos_evilguy--;
						}
						if(event.keyboard.keycode == ALLEGRO_KEY_RIGHT && evilguy->next){
							evilguy = evilguy->next;
							pos_evilguy++;
						}
					}
					else{
						blocker->disp(pos, 4, false);
						if(event.keyboard.keycode == ALLEGRO_KEY_UP) evilguy = nextopponent->fperm[4];
					}
					if(evilguy){
						evilguy->disp(pos_evilguy, 5, true);
					}
					else{
						blocker->disp(pos, 4, true);
					}
					if(event.keyboard.keycode == ALLEGRO_KEY_SPACE || event.keyboard.keycode == ALLEGRO_KEY_ENTER) break;
				}
			}
			blocker->assigned_bl = (Creature*) evilguy;
			if(evilguy){
				Creature* evilcrea = (Creature*) evilguy;
				evilcrea->nb_blockers += 1;

				Permanent* prevbloc = 0;
				for(Creature* iter = (Creature*) evilguy->prev; iter!=0; iter = (Creature*) iter->prev){ //searching for first predecessor to have a blocker
					if(iter->assigned_bl != 0) prevbloc = (Permanent*) iter->assigned_bl;
				}
				Creature* i2 = blocker;
				blocker = (Creature*) blocker->next;
				i2->pop(); //removing i2 from current position
				if(prevbloc) prevbloc->next = (Permanent*) i2;
				else fperm[4] = (Permanent*) i2;
				i2->prev = prevbloc;

				Permanent* nextbloc = 0;
				for(Creature* iter = evilcrea; iter!=0; iter = (Creature*) iter->next){
					if(iter->assigned_bl != 0) nextbloc = (Permanent*) iter->assigned_bl;
				}
				if(nextbloc) nextbloc->prev = i2;
				i2->next = nextbloc;

				evilcrea->assigned_bl = i2;
			}
			else blocker = (Creature*) blocker->next;
		}
		else blocker = (Creature*) blocker->next;
		pos++;
	}
}

void Player::damagestep(){
	state+=32;
	if(!(fperm[4])) return;
	Creature* iter = (Creature*) fperm[4];
	while(1){
		if(iter->nb_blockers == 0){
			iter->hit((Damageable*) nextopponent);
		}
		if(iter->nb_blockers == 1){
			iter->hit((Damageable*) iter->assigned_bl);
			(iter->assigned_bl)->hit(iter);
		}
		else{
			Creature* itb = iter->assigned_bl;
			for(int i=0; i<iter->get_power() && iter->nb_blockers > 0; itb= (Creature*) itb->next){
				#define REPLACE_THIS_VARIABLE 0
				metagame->message("How many damage to: ?"); itb->describe(REPLACE_THIS_VARIABLE);
				int a; //mvwscanw(winzones[6],1,0, "%d", &a);
				if(a + i > iter->get_power()) a = iter->get_power();
				itb->damage(a);
				itb->hit(iter);
				i += a;
				iter->nb_blockers--;
			}
		}
		iter->assigned_bl = 0;
		if(iter->next == 0) break;
		iter = (Creature*) iter->next;
	}
	takebackfighters(iter);
	nextopponent->takebackfighters(0);
	metagame->statebasedactions();
	metagame->disp();
}

void Player::takebackfighters(Permanent* last_fighter){
	if(!last_fighter){ //for blockers
		if(fperm[4]){
			for(last_fighter = fperm[4]; last_fighter->next != 0; last_fighter = last_fighter->next);
		}
		else return;
	}
	last_fighter->next = fperm[3];
	if(fperm[3]) fperm[3]->prev = last_fighter;
	fperm[3] = fperm[4];
	fperm[4] = 0;
	//wclear(winzones[5]);
}
