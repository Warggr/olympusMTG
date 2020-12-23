#include "12defaultUI.h"
#include "../HFiles/8options.h"
#include <iostream>

Target* Default_ui::iterate(bool needstarget, Player** pl, char returntypeflags){
	bool hasroot = true;
	char direction = olympus::directions::LEFT;
	Target* ret;
	int gridy = 0;
	int gridz = 0;
	while(1){
		if(gridy == 0){
			if(gridz == 0 || gridz == 11) ret = (Target*) (*pl)->iterate_self(&direction);
			else ret = (Target* const) (*pl)->iterate_boardsubzone(mousey, &direction, gridz-1, !needstarget && hasroot);
		}
		else if(gridy == 1) ret = (Target*) god.game->iterate_stack(mousez, &direction);

		if(ret != 0){
			if(returntypeflags & (ret->target_type())){
				if(needstarget || hasroot){
					return ret;
				}
				else god.myIO->message("Can't activate your opponents's abilities");
			}
			else god.myIO->message("Wrong type of target");
		}
		else{
			if(direction == olympus::directions::NOTONOBJECT){
				direction = myIO->get_direction_key();
				if(direction != olympus::directions::MOUSE){
					gridy = 0;
					gridz = 0;
				}
				else normalize_gridy_gridz(&gridy, &gridz);
			}
			switch(direction){
				case olympus::directions::LEFT: gridy -= 1; break;
				case olympus::directions::RIGHT: gridy += 1; break;
				case olympus::directions::UP: gridz += 1; break;
				case olympus::directions::DOWN: gridz -= 1; break;
				case olympus::directions::MOUSE:
					normalize_gridy_gridz(&gridy, &gridz);
					god.myIO->message("Mouse detected");
					break;
				default: if(!needstarget) return 0;
			}
		}
		if(direction != olympus::directions::MOUSE){mousey = 0; mousez = 0; } 
		if(gridy == 2){
			if(!needstarget) return 0;
			else gridy = 1;
		}
		if(gridy == -1) gridy += 1;
		if(gridz < 0) gridz = 0;
		if(gridz > 11) gridz = 11;
		if((gridz == 5 && direction == olympus::directions::DOWN) || (gridz == 6 && direction == olympus::directions::UP)){
			(*pl) = (*pl)->nextopponent;
			hasroot = !hasroot;
		}
	}
}

Option* Default_ui::choose_opt(float zOffset, bool sorceryspeed, Option* iter, Player* asker, int metapos){ //asks user to choose option and pops that option
	int y, z, dy, dz;
	optionZone->get_coordinates(&y, &z, &dy, &dz);
	metapos--;
	int pos = 0;
	while(1){
		char dir = myIO->get_direction_key();
		iter->disp(y + pos*dy, z + pos*dz, false, iter->iscastable(asker));
		switch(dir){
			case olympus::directions::DOWN: asker->get_down(iter, pos, metapos); break;
			case olympus::directions::UP: asker->get_up(iter, pos, metapos); break;
			case olympus::directions::SPACE:
				clear_opts();
				return NULL;
			case olympus::directions::ENTER:
				if(iter->iscastable(asker)){ //ENTER
					iter->check_and_pop(metapos, asker);
					clear_opts();
					return iter;
				}
				else myIO->message("This opportunity can't be cast");
				break;
			case olympus::directions::MOUSE:
				if(mousey > boardY + leftbarY){
					while(z + (pos+1)*dz < mousez){
						if(!asker->get_down(iter, pos, metapos)) break;
					}
					while(z + pos*dz > mousez){
						if(!asker->get_up(iter, pos, metapos)) break;
					}
					break;
				}
				//else continue to:
			case olympus::directions::LEFT:{ //when mouse is called, iterate by default. Iterate will know what to do and maybe give back control asap.
				Player* pl = asker;
				Target* activated = iterate(false, &pl, 0x40);
				if(activated){
					if(pl == asker){
						(dynamic_cast<Permanent*> (activated))->activate();
					}
					else myIO->message("Can't activate your opponent's abilities");
				}
			} break;
		}
		iter->disp(y + pos*dy, z+pos*dz, true, iter->iscastable(asker));
		myIO->refresh_display();
	}
	return 0;
}