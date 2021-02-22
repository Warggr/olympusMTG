#include "12defaultUI.h"
#include "../include/3player.h" //for iterating player's elements
#include "../include/4permanents.h"
#include "../include/7game.h"
#include "../HFiles/8options.h"

void Default_ui::deadzone(){
	direction = myIO->get_direction_key();
	if(direction == MOUSE) normalize_gridy_gridz();
	else{ gridy = 0; gridz = 0; direction = NOT_RECOGNIZED; }
}

Target* Default_ui::iterate(bool needstarget, Player** pl, char returntypeflags){
	//std::cout << "Started global Iterate" << std::endl;
	bool hasroot = true;
	Target* ret;
	while(1){
		//1. getting to fitting screen position
		if(direction == MOUSE){
			normalize_gridy_gridz();
		}
		if(gridy == 2){
			if(!needstarget) return 0;
			else if(myIO->gmouseActive()) deadzone();
			else gridy = 1;
		}
		if(gridy == -1) gridy += 1;
		if(gridz < 0) gridz = 0;
		if(gridz > 11) gridz = 11;
		if((gridz == 5 && direction == DOWN) || (gridz == 6 && direction == UP)){ //TODO: modify this condition to be valid for mouse
			(*pl) = (*pl)->nextopponent;
			hasroot = !hasroot;
		}
		//2. call children iterate and get target
		if(gridy == 0){
			if(gridz == 0 || gridz == 11) ret = (Target*) (*pl)->iterate_self(direction);
			else ret = (Target* const) (*pl)->iterate_boardsubzone(myIO->gmouseY(), direction, gridz-1, !needstarget && hasroot);
		}
		else if(gridy == 1) ret = (Target*) god.game->iterate_stack(myIO->gmouseZ(), direction);
		//3. If fitting target found, return it.
		if(ret != 0){
			if(returntypeflags & (ret->target_type())){
				if(needstarget || hasroot){
					return ret;
				}
				else myIO->message("Can't activate your opponents's abilities");
			}
			else myIO->message("Wrong type of target");
		}
		//4. else if direction returned, move.
		else{
			switch(direction){
				case UP: gridz += 1; break;
				case DOWN: gridz -= 1; break;
				case LEFT: gridy -= 1; break;
				case RIGHT: gridy += 1; break;
				case MOUSE: normalize_gridy_gridz(); break;
				default: if(!needstarget) return 0; //SPACE or NOT_RECOG
			}
		}
	}
}

bool go_up(Option*& iter, int& pos, int& metapos, Player* asker, Option*& lower_border){
	if(iter->prev == 0) return false;
	--pos;
	if(iter == asker->myoptions[metapos]){
		lower_border = iter;
		--metapos;
		if(metapos == 2 && asker->myoptions[metapos] == iter) --metapos; //2 and 3 could show to the same element. This can't happen with 0 and 1 because else we couldn't go up at all
		for(metapos = metapos; asker->myoptions[metapos] == 0; --metapos);
	}
	iter = iter->prev;
	return true;
}

bool go_down(Option*& iter, int& pos, int& metapos, Player* asker, Option*& lower_border){
	if(iter->next == 0) return false;
	++pos;
	iter = iter->next;
	if(iter == lower_border){
		metapos++;
		asker->first_option(metapos);
		lower_border = asker->first_option(metapos+1);
	}
	return true;
}

Option* Default_ui::choose_opt(bool sorceryspeed, Player* asker){ //asks user to choose option and pops that option
	int metapos = 0;
	Option* iter = asker->first_option(metapos);
	int dy, dz;
	Rect rect = optionZone->get_coordinates(&dy, &dz);
	int pos = 0;
	Option* lower_border = asker->first_option(metapos+1);
	if(myIO->gmouseActive()){
		while(rect.z + (pos+1)*dz < myIO->gmouseZ()){
			if(go_down(iter, pos, metapos, asker, lower_border) == false) break;
		}
		while(rect.z + pos*dz > myIO->gmouseZ()){
			if(go_up(iter, pos, metapos, asker, lower_border) == false) break;
		}
	}
	while(1){
		direction = myIO->get_direction_key();
		iter->disp(rect.y + pos*dy, rect.z + pos*dz, rect.width, false, iter->iscastable(asker));
		switch(direction){
			case DOWN: go_down(iter, pos, metapos, asker, lower_border); break;
			case UP: go_up(iter, pos, metapos, asker, lower_border); break;
			case BACK:
				clear_opts();
				return NULL;
			case ENTER:
				if(iter->iscastable(asker)){ //ENTER
					clear_opts();
					iter->check_and_pop(metapos, asker);
					return iter;
				}
				else myIO->message("This opportunity can't be cast");
				break;
			case MOUSE:
				if(myIO->gmouseY() > boardW + leftbarW){
					bool gotthere = true;
					while(rect.z + (pos+1)*dz < myIO->gmouseZ()){
						if(iter->next == 0){ gotthere = false; break; }
						iter = iter->next; pos++;
					}
					while(rect.z + pos*dz > myIO->gmouseZ()){
						if(iter->prev == 0){ gotthere = false; break; }
						iter = iter->prev; pos--;
					}
					if(gotthere) break; //don't iterate
					else deadzone(); //wait for a suitable position, then iterate
				} //if we're below option zone, or if we went left, continue to: 
			case LEFT:{
				Player* pl = asker;
				direction = LEFT;
				Target* activated = iterate(false, &pl, 0x40);
				if(activated){
					if(pl == asker){
						(dynamic_cast<Permanent*> (activated))->activate();
					}
					else myIO->message("Can't activate your opponent's abilities");
				}
			} break;
			default: break;
		}
		iter->disp(rect.y + pos*dy, rect.z+pos*dz, rect.width, true, iter->iscastable(asker));
	}
	return 0;
}