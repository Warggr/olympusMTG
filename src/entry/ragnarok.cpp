#include ".header_link.h"
#include "../include/2cards.h"
#include "../HFiles/8options.h"
#include "../HFiles/9modifs.h"
#include "../HFiles/12abilities.h"
#include "../Yggdrasil/headB_board.h"

/* Ragnarok is called when the game ends. The goal is to simply free all objects.
 * Ragnarok should not be called during initialization, when not everything's been allocated (or should it?)
 * As a general rule/ good practice, objects do not delete their siblings. Parents are expected to handle iterating and deleting themselves*/

Player::~Player(){
	god.gdebug(DBG_RAGNAROK) << "This is the end... Hold your breath and count, to ten...\n";
	//deleting permanents is done automatically when their lists are deleted
    Option* first = first_option(0);
    if(first) first->ragnarok();
}

template <typename T>
void CollectionTN<T>::ragnarok_collectiontn() { //delete its node children (not the leaves)
    for(auto iter = children.begin(); iter != children.end(); iter++){
        (*iter)->ragnarok_collectiontn();
        delete *iter;
    }
}

template <typename T>
void CollectionTN<T>::full_ragnarok_boardzone(){
    auto iter = pure_children_first;
    while(iter != pure_children_last){
        auto i2 = iter;
        iter = iter->next;
        delete i2;
    }
    ragnarok_collectiontn();
}

BoardN::~BoardN(){
    mylands.full_ragnarok_boardzone();
    myartos.full_ragnarok_boardzone();
    mycreas.full_ragnarok_boardzone();
    mysuperfriends.full_ragnarok_boardzone();
}

CardZone::~CardZone(){
    for(auto card : cards) delete card;
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

void Permanent::ragnarok_perm(){
    delete source;
    Option* iter = first_actab;
    while(iter != 0){
        auto i2 = iter;
        iter = iter->next;
        delete i2;
    }
}

void SpellOption::ragnarok() {
    delete source;
    if(next) next->ragnarok();
    delete this;
}
PermOption::~PermOption(){ delete effects; }