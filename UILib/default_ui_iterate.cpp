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

Option* Default_ui::choose_opt(bool sorceryspeed, Option* iter, Player* asker, int metapos){ //asks user to choose option and pops that option
	//std::cout << "Started global ChooseOpt" << std::endl;
	int dy, dz;
	Rect rect = optionZone->get_coordinates(&dy, &dz);
	int pos = 0;
	if(myIO->gmouseActive()){
		//std::cout << "Mouse is active:";
		while(rect.z + (pos+1)*dz < myIO->gmouseZ()){
			//std::cout << "Going down - ";
			if(iter->next == 0) break;
			++pos; iter = iter->next;
		}
		while(rect.z + pos*dz > myIO->gmouseZ()){
			if(iter->prev == 0) break;
			--pos; iter = iter->prev;
		}
	}
	while(1){
		direction = myIO->get_direction_key();
		iter->disp(rect.y + pos*dy, rect.z + pos*dz, rect.width, false, iter->iscastable(asker));
		switch(direction){
			case DOWN: if(iter->next){iter = iter->next; pos++; } break;
			case UP: if(iter->prev){iter = iter->prev; pos--; } break;
			case BACK:
				clear_opts();
				return NULL;
			case ENTER:
				if(iter->iscastable(asker)){ //ENTER
					iter->check_and_pop(metapos, asker);
					clear_opts();
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