#include ".header_link.h"
#include "../Yggdrasil/headB_board.h"
#include <iostream>

Permanent* Player::iterate_boardsubzone(float yOffset, DirectioL& direction, int zzone, bool isactivation){
	switch(zzone){
		case 0:
		case 9:
			return iterate_boardsubzone<Land>(yOffset, direction, &myboard.mylands, permUI[0], isactivation);
		case 1:
		case 8:
			return iterate_boardsubzone<Artifact>(yOffset, direction, &myboard.myartos, permUI[1], isactivation);
		case 2:
		case 7:
			return iterate_boardsubzone<Planeswalker>(yOffset, direction, &myboard.mysuperfriends, permUI[2], isactivation);
		case 3:
		case 6:
			return iterate_boardsubzone<Creature>(yOffset, direction, &myboard.mycreas, permUI[3], isactivation);
		case 4:
		case 5:
			return iterate_boardsubzone<Creature>(yOffset, direction, myboard.myattackers, permUI[4], isactivation);
		default:
			exit(1);
	}
}

template <class PermType>
PermType* Player::iterate_boardsubzone(float offset, DirectioL& direction, PContainer<PermType>* perms, UIElement* ui, bool isactivation){
	if(perms == 0 || perms->empty()){
		if(direction == MOUSE) god.myUI->deadzone();
		return 0;
	}
	iterator<PermType, false> iter = perms->begin();
	int y, z, yOff, zOff, yWidth, zHeight;
	ui->get_coordinates(&y, &z, &yOff, &zOff, &yWidth, &zHeight);
	while(y < offset - yOff){
		y += yOff;
		iter++;
		if(iter == perms->end()){
			god.myUI->deadzone();
			return 0;
		}
	}
	iter->disp(y, z, yWidth, zHeight, true);
	while(1){
		//god.myIO->refresh_display();
		DirectioL getkey = god.myIO->get_direction_key();
		iter->disp(y, z, yWidth, zHeight, false);
		switch(getkey){
			case NOT_RECOGNIZED:
				god.myIO->message("Please press a direction key, SPACE or ENTER");
				break;
			case UP:
			case DOWN:
			case MOUSE:
			case BACK:
				direction = getkey;
				return 0;
			case LEFT:
				direction = LEFT;
				y -= yOff;
				if(iter == perms->begin()) return 0;
				else iter--;
				break;
			case RIGHT:
				direction = RIGHT;
				iter++;
				if(iter == perms->end()) return 0;
				y += yOff;
				break;
			case ENTER:
				direction = ENTER;
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

Player* Player::iterate_self(DirectioL& direction){
	std::cout << "Started iterating player" << std::endl;
	god.myIO->message("Iterating player");
	while(1){
		disp_header(true);
		direction = god.myIO->get_direction_key();
		disp_header(false);
		switch(direction){
			case NOT_RECOGNIZED:
				god.myIO->message("Please press a direction key, SPACE or ENTER");
				break;
			case MOUSE:
			case DOWN:
			case UP:
			case LEFT:
			case RIGHT:
			case BACK: //SPACE
				return 0;
			case ENTER: //ENTER
				return this;
		}
	}
}

Resolvable* Game::iterate_stack(float zImposed, DirectioL& direction){
	std::cout << "Started iterating stack" << std::endl;
	if(stack.empty()){
		if(direction == MOUSE) god.myUI->deadzone();
		//direction = LEFT;
		return 0;
	}
	std::forward_list<Resolvable*>::iterator ret = stack.begin();
	std::forward_list<std::forward_list<Resolvable*>::iterator> counterstack;
	int y, z, yOffset, zOffset, ySize, zSize;
	stack_ui->get_coordinates(&y, &z, &yOffset, &zOffset, &ySize, &zSize);
	while(z + zOffset < zImposed){
		z += zOffset;
		counterstack.push_front(ret);
		ret++;
		if(ret == stack.end()){
			god.myUI->deadzone();
			return 0;
		}
	}
	(*ret)->disp(y, z, ySize, zSize, true);
	while(1){
		direction = god.myIO->get_direction_key();
		(*ret)->disp(y, z, ySize, zSize, false);
		switch(direction){
			case NOT_RECOGNIZED:
				god.myIO->message("Please press a direction key, SPACE or ENTER");
				break;
			case RIGHT:
			case LEFT:
			case MOUSE:
			case BACK: //SPACE
				return 0;
			case ENTER: //ENTER
				return *ret;
			case DOWN:{
				auto nxt = ret; nxt++;
				if(nxt != stack.end()){
					counterstack.push_front(ret);
					y += yOffset; z += zOffset;
					ret++;
				}
				break;
			}
			case UP:{
				direction = UP;
				if(counterstack.empty()) return 0;
				else{
					ret = counterstack.front();
					counterstack.pop_front();
					y -= yOffset; z -= zOffset;
				}
			}
		}
		(*ret)->disp(y, z, ySize, zSize, true);
	}
	return 0;
}