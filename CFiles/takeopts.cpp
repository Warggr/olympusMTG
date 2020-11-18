#include <cstdio>
#include <cstdlib>
#include "../HFiles/classes.h"

//Player calls choicephase.
//Player::choicephase calls choose_and_use_opt. Calls check_and_pop. Popped option is returned and cast (aka added to stack, or land put in play)
//Function returns 0 if Default was cast.
//Player::choicephase lets opponent choose_and_use_opt afterwards.
//When both return 0, Player::choicephase resolves the first Option of the stack via Spell::resolve. Then new alternating choose_and_use_opt
//When both pass and the stack is empty, Player::choicephase returns.

void Player::choicephase(bool sorceryspeed){
	int i = 0;
	while(1){
		Player* currentprio = this;
		while(metagame->stackisempty() && i==0) if(!choose_and_use_opt(sorceryspeed)) {i=1; currentprio = currentprio->nextopponent; }
		while(i!=2){
			if(currentprio->choose_and_use_opt(0)) i=0;
			else{
				i++;
				currentprio = currentprio->nextopponent;
			}
		} //two passes in a row; resolving first spell
		Resolvable* toresolve = metagame->popfromstack();
		if(toresolve){
			god.gdebug(DBG_TARGETING) << "RESOLVING A SPELL\n";
			toresolve->resolve();
			delete toresolve;
			metagame->statebasedactions();
			i = 0;
			metagame->disp();
		}
		else return; //if there is no first spell, then the phase/step ends
	}
}

bool Player::choose_and_use_opt(bool sorceryspeed){ //returns false if a pass option was chosen
	metagame->statebasedactions(); //"before any player receives priority, state-based actions are done"
	Option* choice = choose_opt(sorceryspeed); //chooses opt, returns 0 if passing was chosen
	if(!choice) return false;
	choice->cast_opt(this); //casts the spell
	metagame->disp_stack();
	disp_zone(0); //the stack might have changed, or the lands
	return true;
}

Option* Player::choose_opt(bool sorceryspeed){ //asks user to choose option and pops that option
	if(!disp_opt(sorceryspeed)){
		clear_opts();
		return NULL;
	}
	Option* iter = 0;
	int metapos;
	for(metapos = 0; !iter && metapos<3; metapos++){
		iter = myoptions[metapos];
	}
	metapos--;
	int pos = 0;
	ALLEGRO_EVENT event;
	while(1){
		al_wait_for_event(god.queue, &event);
		iter->disp(2*pos+3, false, iter->iscastable(this));
		switch(event.type){
			case ALLEGRO_EVENT_KEY_CHAR:
			switch(event.keyboard.keycode){
				case ALLEGRO_KEY_DOWN:
					get_down(&iter, &pos, &metapos); break;
				case ALLEGRO_KEY_UP:
					get_up(&iter, &pos, &metapos); break;
				case ALLEGRO_KEY_ENTER:
					if(iter->iscastable(this)){ //ENTER
						iter->check_and_pop(metapos, this);
						clear_opts();
						return iter;
					}
					else metagame->message("This opportunity can't be cast");
					break;
				case ALLEGRO_KEY_SPACE: //SPACE
					clear_opts();
					return NULL;
				case ALLEGRO_KEY_RIGHT:{
					metagame->message("Right key pressed");
					Player* pl = this;
					Target* activated = iterate(false, &pl, 0x40);
					if(activated){
						metagame->message("Activated possible ability");
						if(pl == this){
							(dynamic_cast<Permanent*> (activated))->activate();
						}
						else metagame->message("Can't activate your opponent's abilities");
					}
					} break;
				default: metagame->message("Please press UP, DOWN or ENTER");
			} break;
			case ALLEGRO_EVENT_DISPLAY_CLOSE:
				god.minimalKill();
				exit(1);
		}
		iter->disp(2*pos+3, true, iter->iscastable(this));
		al_flip_display();
	}
	return 0;
}

void Player::get_down(Option** iter, int* pos, int* metapos){
	if((**iter).next){
		(*pos)++;
		(*iter) = (*iter)->next;
	}
	else{
		int i = (*metapos)+1;
		while(i < NBMYOPTS-1 && myoptions[i] == 0) i++;
		if(myoptions[i]){
			(*metapos) = i;
			(*pos)++;
			(*iter) = myoptions[*metapos];
		}
	}
}

void Player::get_up(Option** iter, int* pos, int* metapos){
	if((**iter).prev){
		(*pos)--;
		(*iter) = (*iter)->prev;
	}
	else{
		int i = (*metapos)-1;
		while(i>=0 && myoptions[i] == 0) i--;
		if(i>=0 && myoptions[i]){
			(*metapos) = i;
			(*pos)--;
			(*iter) = myoptions[*metapos];
			while((*iter)->next) *iter = (*iter)->next;
		}
	}
}

bool Player::disp_opt(bool sorceryspeed){
	bool ret = false;
	int pos = 3;
	dispOptsOfCertainType(&pos, INSTANTOPTS, true); if(pos!=3) ret = true; 
	dispOptsOfCertainType(&pos, SORCERYOPTS, sorceryspeed); if(sorceryspeed && pos!=3) ret = true;
	dispOptsOfCertainType(&pos, LANDOPTS, sorceryspeed && !(state & 16)); if(sorceryspeed && !(state & 16) && myoptions[LANDOPTS]) ret = true;
	dispOptsOfCertainType(&pos, TOOEXPENSIVE, false);

	al_flip_display();
	return ret;
}

void Player::dispOptsOfCertainType(int* pos, int type, bool castable){
	for(Option* iter = myoptions[type]; iter!=0; iter = iter->next){
		iter->disp_in_list(pos, type, this, castable);
	}
}

void Option::disp_in_list(int* pos, char opttype, Player* poss, bool castable){
	disp(*pos, (*pos==3), castable && iscastable(poss)); //already highlight if we're the first option
	(*pos) += 2;
}