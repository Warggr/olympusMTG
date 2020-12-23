#include ".header_link.h"

Permanent* Player::iterate_boardsubzone(float yOffset, char* direction, int zzone, bool isactivation){
	switch(zzone){
		case 0:
		case 9:
			return iterate_boardsubzone<Land>(yOffset, direction, mylands, permUI[0], isactivation);
		case 1:
		case 8:
			return iterate_boardsubzone<Artifact>(yOffset, direction, myartos, permUI[1], isactivation);
		case 2:
		case 7:
			return iterate_boardsubzone<Planeswalker>(yOffset, direction, mysuperfriends, permUI[2], isactivation);
		case 3:
		case 6:
			return iterate_boardsubzone<Creature>(yOffset, direction, mycreas, permUI[3], isactivation);
		case 4:
		case 5:
			return iterate_boardsubzone<Creature>(yOffset, direction, myattackers, permUI[4], isactivation);
		default:
			exit(1);
	}
}

template <class PermType>
PermType* Player::iterate_boardsubzone(float offset, char* direction, std::list<PermType>& perms, UIElement* ui, bool isactivation){
	if(perms.empty()){
		if(*direction == olympus::directions::MOUSE) *direction = olympus::directions::NOTONOBJECT;
		return 0;
	}
	typename std::list<PermType>::iterator iter = perms.begin();
	int y, z, yOff, zOff, yWidth, zHeight;
	ui->get_coordinates(&y, &z, &yOff, &zOff, &yWidth, &zHeight);
	while(y < offset - yOff){
		y += yOff;
		iter++;
		if(iter == perms.end()){
			*direction = olympus::directions::NOTONOBJECT;
			return 0;
		}
	}
	iter->disp(y, z, yWidth, zHeight, true);
	while(1){
		god.myIO->refresh_display();
		char getkey = god.myIO->get_direction_key();
		iter->disp(y, z, yWidth, zHeight, false);
		switch(getkey){
			case olympus::directions::UP:
			case olympus::directions::DOWN:
			case olympus::directions::MOUSE:
				*direction = getkey;
				return 0;
			case olympus::directions::SPACE:
				*direction = 0;
				return 0;
			case olympus::directions::LEFT:
				*direction = olympus::directions::LEFT;
				y -= yOff;
				if(iter == perms.begin()) return 0;
				else iter--;
				break;
			case olympus::directions::RIGHT:
				*direction = olympus::directions::RIGHT;
				iter++;
				if(iter == perms.end()) return 0;
				y += yOff;
				break;
			case olympus::directions::ENTER:
				*direction = 0;
				if(isactivation){
					if(!iter->directactivate()){
						iter->disp(y, z, yWidth, zHeight, true);
					}
					else return &(*iter);
				}
				else return &(*iter);
		}
		iter->disp(y, z, yWidth, zHeight, true);
	}
}

Player* Player::iterate_self(char* direction){
	god.myIO->message("Iterating player");
	while(1){
		disp_header(true);
		god.myIO->refresh_display();
		char getkey = god.myIO->get_direction_key();
		disp_header(false);
		switch(getkey){
			case olympus::directions::MOUSE:
			case olympus::directions::DOWN:
			case olympus::directions::UP:
			case olympus::directions::LEFT:
			case olympus::directions::RIGHT:
				*direction = getkey;
				return 0;
			case olympus::directions::ENTER: //ENTER
				*direction = 0;
				return this;
			case olympus::directions::SPACE: //SPACE
				*direction = 0;
				return 0;
			default:
				god.myIO->message("Please press a direction key, SPACE or ENTER");
				*direction = 0;
				return 0;
		}
	}
}

Resolvable* Game::iterate_stack(float zImposed, char* direction){
	if(stack.empty()){
		//*direction = olympus::directions::LEFT;
		return 0;
	}
	std::forward_list<Resolvable*>::iterator ret = stack.begin();
	std::forward_list<std::forward_list<Resolvable*>::iterator> counterstack;
	int y, z, yOffset, zOffset, ySize, zSize;
	stack_ui->get_coordinates(&y, &z, &yOffset, &zOffset, &ySize, &zSize);
	while(z + zOffset < zImposed){
		y += yOffset; z += zOffset;
		counterstack.push_front(ret);
		ret++;
		if(ret == stack.end()){
			*direction = olympus::directions::NOTONOBJECT;
			return 0;
		}
	}
	(*ret)->disp(y, z, ySize, zSize, true);
	god.myIO->refresh_display();
	while(1){
		int a = god.myIO->get_direction_key();
		(*ret)->disp(y, z, ySize, zSize, false);
		switch(a){
			case olympus::directions::SPACE: //SPACE
				return 0;
			case olympus::directions::ENTER: //ENTER
				return *ret;
			case olympus::directions::DOWN:{
				auto nxt = ret; nxt++;
				if(nxt != stack.end()){
					counterstack.push_front(ret);
					y += yOffset; z += zOffset;
					ret++;
				}
				*direction = olympus::directions::DOWN;
				break;
			}
			case olympus::directions::RIGHT:
			case olympus::directions::LEFT:
				*direction = a;
				return 0;
			case olympus::directions::UP:{
				*direction = olympus::directions::UP;
				if(counterstack.empty()) return 0;
				else{
					ret = counterstack.front();
					counterstack.pop_front();
					y -= yOffset; z -= zOffset;
				}
			}
		}
		(*ret)->disp(y, z, ySize, zSize, true);
		god.myIO->refresh_display();
	}
	return 0;
}