#include "../HFiles/classes.h"

Target* iterate(bool needstarget, Player** pl, char returntypeflags){
	int gridx = 0;
	int gridy = 0;
	bool hasroot = true;
	char direction = 2;
	Target* ret;
	while(1){
		if(gridy == 0){
			if(gridx == 0 || gridx == 11){
				ret = (Target*) (*pl)->iterate_self(&direction);
				(*pl)->metagame->message("Stopped iterating player");
			}
			else{
				if(gridx <= 5){
					ret = (Target*) (*pl)->iterate_boardsubzone(&direction, gridx-1, !needstarget && hasroot);
				}else{
					ret = (Target*) (*pl)->iterate_boardsubzone(&direction, 10-gridx, !needstarget && hasroot);
				}
			}
		}
		else if(gridy == 1){
			ret = (Target*) (*pl)->metagame->iterate_stack(&direction);
		}

		if(ret != 0){
			if(returntypeflags & (ret->target_type())){
				if(needstarget || hasroot){
					return ret;
				}
				else (*pl)->metagame->message("Can't activate your opponents's abilities");
			}
			else (*pl)->metagame->message("Wrong type of target");
		}
		else{
			switch(direction){
				case 1: gridx -= 1; break;
				case -1: gridx += 1; break;
				case 2: gridy += 1; break;
				case -2: gridy -= 1; break;
				default: if(!needstarget) return 0;
			}
		}
		if(gridy == -1){
			if(!needstarget) return 0;
			else gridy = 0;
		}
		if(gridy == 2) gridy -= 1;
		if(gridx < 0) gridx = 0;
		if(gridx > 11) gridx = 11;
		if((gridx == 5 && direction == 1) || (gridx == 6 && direction == -1)){
			(*pl) = (*pl)->nextopponent;
			hasroot = !hasroot;
		}
	}
}

Player* Player::iterate_self(char* direction){
	metagame->message("Iterating player");
	disp_header();
	al_flip_display();
	ALLEGRO_EVENT event;
	while(1){
		al_wait_for_event(god.queue, &event);
		disp_header();
		switch(event.type){
			case ALLEGRO_EVENT_KEY_CHAR:
				switch(event.keyboard.keycode){
					case ALLEGRO_KEY_DOWN:
						*direction = -1;
						return 0;
					case ALLEGRO_KEY_UP:
						*direction = 1;
						return 0;
					case ALLEGRO_KEY_LEFT:
						*direction = -2;
						return 0;
					case ALLEGRO_KEY_ENTER:
						*direction = 0;
						return this;
					case ALLEGRO_KEY_SPACE: //SPACE
						*direction = 0;
						return 0;
					case ALLEGRO_KEY_RIGHT:
						*direction = 2;
						return 0;
					default:
						metagame->message("Please press a direction key, SPACE or ENTER");
						*direction = 0;
						return 0;
				}
				case ALLEGRO_EVENT_DISPLAY_CLOSE:
					god.minimalKill();
					exit(1);
		}
	}
}

Permanent* Player::iterate_boardsubzone(char* direction, int xzone, bool isactivation){
	metagame->message("Iterating boardsubzone");
	int yperm = 0;
	Permanent* iter = fperm[xzone];
	if(!iter) return 0;
	iter->disp(yperm, xzone, true);
	while(1){
		al_flip_display();
		ALLEGRO_EVENT event;
		al_wait_for_event(god.queue, &event);
		iter->disp(yperm, xzone, false);
		switch(event.type){
			case ALLEGRO_EVENT_KEY_CHAR:
				switch(event.keyboard.keycode){
					case ALLEGRO_KEY_UP:
						*direction = 1;
						return 0;
					case ALLEGRO_KEY_DOWN:
						*direction = -1;
						return 0;
					case ALLEGRO_KEY_SPACE:
						*direction = 0;
						return 0;
					case ALLEGRO_KEY_LEFT:
						*direction = -2;
						yperm -= 1;
						if(yperm == -1) return 0;
						iter = iter->prev;
						break;
					case ALLEGRO_KEY_RIGHT:
						*direction = 2;
						if(!iter->next) return 0;
						iter = iter->next;
						yperm += 1;
						break;
					case ALLEGRO_KEY_ENTER:
						*direction = 0;
						if(isactivation){
							if(!iter->directactivate()){
								iter->disp(yperm, xzone, true);
							}
							else return iter;
						}
						else return iter;
				} break;
			case ALLEGRO_EVENT_DISPLAY_CLOSE:
				god.minimalKill();
				exit(1);
		}
		iter->disp(yperm, xzone, true);
	}
}

Resolvable* Game::iterate_stack(char* direction){
	Resolvable* ret = stack;
	if(ret == 0){
		*direction = -2;
		return 0;
	}
	/*wattron(winzones[1], A_STANDOUT);
	ret->describe(winzones[1]);
	wattroff(winzones[1], A_STANDOUT);
	wrefresh(winzones[1]);
	while(1){
		int a = wgetch(winzones[2]);
		disp_stack();
		switch(a){
			case 32: //SPACE
				return 0;
			case 10: //ENTER
				return ret;
			case KEY_DOWN:
				if(ret->next) ret = ret->next;
				*direction = -1;
				break;
			case KEY_RIGHT:
				*direction = 2; return 0;
			case KEY_LEFT:
				*direction = -2; return 0;
			case KEY_UP:
				*direction = -1;
				if(stack == ret) return 0;
				else{
					Resolvable* i;
					for(i = stack; stack->next != ret; i = i->next);
					ret = i;
				}
		}
		wmove(winzones[1], 0, 0);
		for(Resolvable* iter = stack; iter != ret; iter = iter->next) iter->describe(winzones[1]);
		wattron(winzones[1], A_STANDOUT);
		ret->describe(winzones[1]);
		wattroff(winzones[1], A_STANDOUT);
		wrefresh(winzones[1]);
	}*/
	return 0;
}