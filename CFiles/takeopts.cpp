#include ".header_link.h"
#include "../HFiles/8options.h"
#include "../HFiles/9modifs.h"
#include <iostream>

void verify_chain_integrity(Option** myoptions){
	int i; Option* iter = 0;
	for(i = 0; i < NBMYOPTS && myoptions[i] == 0; i++);
	if(i == NBMYOPTS) return;
	if(myoptions[i]->prev != 0){
		god.call_ragnarok();
		std::cout << "Error: first element of chain has prev pointer\n";
		exit(1);
	}
	for(iter = myoptions[i]; iter->next != 0; iter = iter->next){
		if(iter->next->prev != iter){
			god.call_ragnarok();
			std::cout << "Error: inconsistency in the chain at ";
			if(iter->prev) std::cout << "<" << iter->prev->tag << "("; else std::cout << "<0(";
			std::cout << iter->tag << ")";
			if(iter->next) std::cout << iter->next->tag << ">"; else std::cout << "0>";
			exit(1);
		}
		if(iter->exists == false){
			god.call_ragnarok();
			std::cout << "Error: deleted element still in the chain\n";
			exit(1);
		}
		if(iter == iter->next){
			god.call_ragnarok();
			std::cout << "Error: circular reference\n";
			exit(1);
		}
	}
	if(iter != myoptions[NBMYOPTS]){
		god.call_ragnarok();
		std::cout << "Error: last element does not point to last element\n";
		exit(1);
	}
	for(int i = 0; i<=NBMYOPTS; i++){
		if(myoptions[i] != 0 && myoptions[i]->exists == false){
			god.call_ragnarok();
			std::cout << "Error: deleted element still pointed by parent\n";
			exit(1);
		}
	}
	std::cout << "Chain integrity OK\n";
}

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
	verify_chain_integrity(myoptions);
	if(!disp_opt(sorceryspeed)){
		god.myUI->clear_opts();
		return false;
	}
	Option* iter = 0;
	int metapos;
	for(metapos = 0; iter == 0 && metapos<5; metapos += 2){ //there exists a castable opt; it must be at 0(instant), 2 (sorcery), or 4 (land)
		iter = myoptions[metapos];
	}
	Option* choice = god.myUI->choose_opt(sorceryspeed, iter, this, metapos); //chooses opt and pops it, returns 0 if passing was chosen
	verify_chain_integrity(myoptions);
	if(!choice) return false;
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

bool Player::disp_opt(bool sorceryspeed) const {
	bool ret = false;
	int dy, dz;
	Rect rect = optZone->get_coordinates(&dy, &dz);
	bool not_moved = true;
	dispOptsOfCertainType(rect, dy, dz, not_moved, 0, true);
		if(!not_moved) ret = true;
	dispOptsOfCertainType(rect, dy, dz, not_moved, 2, sorceryspeed);
		if(sorceryspeed && !not_moved) ret = true;
	dispOptsOfCertainType(rect, dy, dz, not_moved, 4, sorceryspeed && !(state & 16));
		if(sorceryspeed && !(state & 16) && myoptions[LANDOPTS]) ret = true;

	return ret;
}

void Player::dispOptsOfCertainType(Rect& zone, int dy, int dz, bool& not_moved, int type, bool castable) const {
	for(Option* iter = myoptions[type]; iter != 0 && iter!=myoptions[type+2]; iter = iter->next){
		iter->disp(zone.yy(), zone.z, zone.width, not_moved, castable && iter->iscastable(this));
		zone.shift(dy, dz);
		not_moved = false;
	}
}