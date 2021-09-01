#include "oracles/classes/8options.h"
#include "3player.h"
#include "7game.h"
#include "gameplay/resolvables/stack.h"
#include <iostream>

//Player calls choicephase.
//Player::choicephase calls choose_and_use_opt. Calls check_and_pop. Popped option is returned and cast (aka added to stack, or land put in play)
//Function returns 0 if Default was cast.
//Player::choicephase lets opponent choose_and_use_opt afterwards.
//When both return 0, Player::choicephase resolves the first EmptyOption of the stack via Spell::resolve. Then new alternating choose_and_use_opt
//When both pass and the stack is empty, Player::choicephase returns.

void Player::choicephase(bool sorceryspeed){
	//optZone->erase_background(god.myIO);
	while(true){
		int i = 0;
		Player* currentprio = this;
		Game::god->stateBasedActions(); //"before any local receives priority, state-based actions are done"
		add_triggers_to_stack(); nextopponent->add_triggers_to_stack();
		while(Stack::god->stackIsEmpty() && i==0){
			if(!choose_and_use_opt(sorceryspeed)) {i=1; currentprio = currentprio->nextopponent; }
		}
		while(i!=2){
			Game::god->stateBasedActions();
			if(currentprio->choose_and_use_opt(false)) i=0;
			else{
				i++;
				currentprio = currentprio->nextopponent;
			}
		} //two passes in a row; resolving first spell
		auto toresolve = Stack::god->popFromStack();
		if(toresolve){
			gdebug(DBG_TARGETING) << "RESOLVING A SPELL\n";
			toresolve->resolve();
		}
		else return; //if there is no first spell, then the phase/step ends
	}
}

bool Player::choose_and_use_opt(bool sorceryspeed){ //AKA "giving priority". Returns false if a pass option was chosen
	//verify_chain_integrity(myoptions);
	if(!hasOptions(sorceryspeed)) return false;
	uptr<OptionAction> choice = agent.chooseOpt(sorceryspeed, this); //chooses opt, pops it and returns it, returns 0 if passing was chosen
	if(!choice) return false;
	//verify_chain_integrity(myoptions);
	choice->cast_opt(this); //casts the spell and deletes the option
	return true;
}

bool Player::add_triggers_to_stack(){
	if(prestack.empty()) return false;
	//TODO: the player may order his triggers
	while(!(prestack.empty())){
		auto res = std::make_unique<Resolvable>(this, prestack.front().preRes, prestack.front().origin->getTarget());
		Stack::god->addToStack(std::move(res));
		prestack.pop_front();
	}
	return true;
}

bool Player::hasOptions(bool sorceryspeed) const {
    (void) sorceryspeed; //TODO
	/*if(!sorceryspeed and myoptions[0] == 0) return false;
	else if(myoptions[2] != 0) return true;
	else if(!hasplayedland() and myoptions[LANDOPTS]) return true; */
	return false;
}

/*void Player::disp_opt(bool sorceryspeed) const {
	int dy, dz;
	Rect rect = optZone->get_coordinates(&dy, &dz);
	int zone = 0;
	bool castable = true, not_moved = true;
	EmptyOption* iter = first_option(&zone);
	EmptyOption* next = first_option(zone);
	for(; iter != 0; iter = iter->next){
		iter->disp(rect.y, rect.z, rect.width, not_moved, castable);
		not_moved = false;
		rect.shift(dy, dz);
		if(iter == next){
			zone++;
			if(zone == 1 || zone == 3) castable = false;
			else if(zone == 4) castable = sorceryspeed and !hasplayedland();
			else if(zone == 2) castable = sorceryspeed;
			next = first_option(zone);
		}
	}
}*/