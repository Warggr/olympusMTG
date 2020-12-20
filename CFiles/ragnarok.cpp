#include "../HFiles/olympus_main.h"

Game::~Game(){
	god.gdebug(DBG_RAGNAROK) << "Destroying the game...\n";
	for(int i=0; i<2; i++){
		delete players[i];
	}
	delete stack_ui;
	delete logbook_ui;
	god.minimalKill();
}

void externVarContainer::minimalKill(){
	delete myIO;
	delete myUI;
	debug_log.close();
	verbose_debug.close();
}

Player::~Player(){
	god.gdebug(DBG_RAGNAROK) << "This is the end... Hold your breath and count, to ten...\n";
	//deleting permanents is done automatically when their lists are deleted
	for(int i=0; i<3; i++){
		if(myoptions[i]) myoptions[i]->ragnarok();
	}
}

Target::~Target(){
	god.gdebug(DBG_RAGNAROK | DBG_TARGETING) << "Deleting Target" << this << ".\n";
	for(auto iter = to_target.begin(); iter != to_target.end(); iter++){
		(*iter)->valid = false;
	}
}

Resolvable::~Resolvable(){
	god.gdebug(DBG_RAGNAROK | DBG_TARGETING) << "Deleting Resolvable" << this << ". Deleting all of its Targeter objects.\n";
	delete[] list_of_targets;
}

Targeter::~Targeter(){
	god.gdebug(DBG_RAGNAROK | DBG_TARGETING) << " Deleting Targeter" << this << ". Removing any Targeters from its targets.\n";
	if(valid) content->remove_persecutor(this); //telling its own death to the permanent it targeted
}