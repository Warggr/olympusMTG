#include ".header_link.h"
#include "../HFiles/8options.h"
#include "../HFiles/9modifs.h"

/* Ragnarok is called when the game ends. The goal is to simply free all objects.
 * Ragnarok should not be called during initialization, when not everything's been allocated (or should it?)
 * As a general rule/ good practice, objects do not delete their siblings. Parents are expected to handle iterating and deleting themselves*/

Player::~Player(){
	god.gdebug(DBG_RAGNAROK) << "This is the end... Hold your breath and count, to ten...\n";
	//deleting permanents is done automatically when their lists are deleted
	for(int i=0; i<3; i++){
		if(myoptions[i]) myoptions[i]->ragnarok();
	}
}

template <typename T>
void CollectionTN<T>::ragnarok_collectiontn() {
    for(auto iter = children.begin(); iter != children.end(); iter++){
        delete *iter;
    }
}

template <typename T>
void CollectionTN<T>::full_ragnarok_boardzone(){
    auto iter = pure_children_first;
    do {
        auto i2 = iter;
        iter = iter->get_next();
        delete i2;
    } while(iter != pure_children_last);
    ragnarok_collectiontn();
    delete this;
}

BoardN::~BoardN(){
    mylands.full_ragnarok_boardzone();
    myartos.full_ragnarok_boardzone();
    mycreas.full_ragnarok_boardzone();
    mysuperfriends.full_ragnarok_boardzone();
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
};