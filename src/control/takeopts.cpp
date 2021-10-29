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
		addTriggersToStack(); nextopponent->addTriggersToStack();
		while(Stack::god->stackIsEmpty() && i==0){
			if(!chooseAndUseOpt(sorceryspeed)) {i=1; currentprio = currentprio->nextopponent; }
		}
		while(i!=2){
			Game::god->stateBasedActions();
			if(currentprio->chooseAndUseOpt(false)) i=0;
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

bool Player::chooseAndUseOpt(bool sorceryspeed){ //AKA "giving priority". Returns false if a pass option was chosen
	if(!myOptions.hasOptions(sorceryspeed)) return false;
	Option* choice = agent.chooseOpt(sorceryspeed, this); //chooses opt, pops it and returns it, returns 0 if passing was chosen
	if(!choice) return false;
	choice->castOpt(this); //casts the spell and deletes the option
	return true;
}

bool Player::addTriggersToStack(){
	if(prestack.empty()) return false;
	//TODO: the player may order his triggers
	while(!(prestack.empty())){
		auto res = std::make_unique<Resolvable>(this, prestack.front().preRes, prestack.front().origin->getTarget());
		Stack::god->addToStack(std::move(res));
		prestack.pop_front();
	}
	return true;
}

bool Player::pay(Cost cost) {
    if(not (manapool >= cost.mana)) return false;
    manapool -= cost.mana;
    return true;
}
