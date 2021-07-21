#include ".header_link.h"
#include "../HFiles/8options.h"
#include "oracles/9modifs.h"
#include <iostream>

//Player calls choicephase.
//Player::choicephase calls choose_and_use_opt. Calls check_and_pop. Popped option is returned and cast (aka added to stack, or land put in play)
//Function returns 0 if Default was cast.
//Player::choicephase lets opponent choose_and_use_opt afterwards.
//When both return 0, Player::choicephase resolves the first Option of the stack via Spell::resolve. Then new alternating choose_and_use_opt
//When both pass and the stack is empty, Player::choicephase returns.

void Player::choicephase(bool sorceryspeed){
	optZone->erase_background(god.myIO);
	while(1){
		int i = 0;
		Player* currentprio = this;
		god.game->statebasedactions(); //"before any player receives priority, state-based actions are done"
		add_triggers_to_stack(); nextopponent->add_triggers_to_stack();
		while(god.game->stackisempty() && i==0){
			if(!choose_and_use_opt(sorceryspeed)) {i=1; currentprio = currentprio->nextopponent; }
		}
		while(i!=2){
			//god.game->statebasedactions(); //Rigorously SBA are checked here, just before giving priority
			//But c'mon, in what case will states have changed just by casting a spell?
			if(currentprio->choose_and_use_opt(false)) i=0;
			else{
				i++;
				currentprio = currentprio->nextopponent;
			}
		} //two passes in a row; resolving first spell
		Resolvable* toresolve = god.game->popfromstack();
		if(toresolve){
			god.gdebug(DBG_TARGETING) << "RESOLVING A SPELL\n";
			toresolve->resolve();
			delete toresolve;
			god.game->disp();
		}
		else return; //if there is no first spell, then the phase/step ends
	}
}

bool Player::choose_and_use_opt(bool sorceryspeed){ //AKA "giving priority". Returns false if a pass option was chosen
	//verify_chain_integrity(myoptions);
	if(has_options(sorceryspeed)){
		god.myUI->clear_opts();
		return false;
	}
	disp_opt(sorceryspeed);
	Option* choice = god.myUI->choose_opt(sorceryspeed, this); //chooses opt, pops it and returns it, returns 0 if passing was chosen
	if(!choice) return false;
	//verify_chain_integrity(myoptions);
	Resolvable* cast = choice->cast_opt(this); //casts the spell and deletes the option
	if(cast){
		god.game->addtostack(cast);
		god.game->disp_stack();
	}
	else{
		disp_zone(0); //the stack might have changed, or the lands
	}
	return true;
}

bool Player::add_triggers_to_stack(){
	if(prestack.empty()) return false;
	//TODO: the player may order his triggers
	while(!(prestack.empty())){
		Resolvable* res = new Resolvable(this, prestack.front().preRes, prestack.front().origin->getTarget());
		god.game->addtostack(res);
		prestack.pop_front();
	}
	return true;
}

bool Player::has_options(bool sorceryspeed) const {
	if(!sorceryspeed and myoptions[0] == 0) return false;
	else if(myoptions[2] != 0) return true;
	else if(!hasplayedland() and myoptions[LANDOPTS]) return true;
	return false;
}

void Player::disp_opt(bool sorceryspeed) const {
	int dy, dz;
	Rect rect = optZone->get_coordinates(&dy, &dz);
	int zone = 0;
	bool castable = true, not_moved = true;
	Option* iter = first_option(&zone);
	Option* next = first_option(zone);
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
}