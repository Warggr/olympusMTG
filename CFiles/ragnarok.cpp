#include "../HFiles/classes.h"
#include <cstdlib>

Game::~Game(){
	god.gdebug(DBG_RAGNAROK) << "Destroying the game...\n";
	for(int i=0; i<2; i++){
		delete players[i];
	}
	god.minimalKill();
}

void externVarContainer::minimalKill(){
	delete images;
	debug_log.close();
	verbose_debug.close();
	//game->deletegraph();
}

Player::~Player(){
	god.gdebug(DBG_RAGNAROK) << "This is the end... Hold your breath and count, to ten...\n";
	for(int i=0; i<5; i++){
		if(fperm[i]) fperm[i]->ragnarok();
	}
	for(int i=0; i<3; i++){
		if(myoptions[i]) myoptions[i]->ragnarok();
	}
}

CardZone::~CardZone(){
	if(first) first->ragnarok();
}

void Card::ragnarok(){
	if(next) delete next;
	delete this;
}

Target::~Target(){
	god.gdebug(DBG_RAGNAROK | DBG_TARGETING) << "Deleting Target" << this << ".\n";
	if(first_to_target) delete first_to_target;
}

ModListNode::~ModListNode(){
	god.gdebug(DBG_RAGNAROK | DBG_TARGETING) << " Deleting ModListNode " << this << ". Its Targeter becomes invalid.\n";
	content->valid = false;
	if(next) delete next;
}

Resolvable::~Resolvable(){
	god.gdebug(DBG_RAGNAROK | DBG_TARGETING) << "Deleting Resolvable" << this << ". Deleting all of its Targeter objects.\n";
	delete[] list_of_targets;
}

Targeter::~Targeter(){
	god.gdebug(DBG_RAGNAROK | DBG_TARGETING) << " Deleting Targeter" << this << ". Removing any Targeters from its targets.\n";
	if(valid) content->remove_persecutor(this); //telling its own death to the permanent it targeted
}