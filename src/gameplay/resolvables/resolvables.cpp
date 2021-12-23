#include "5resolvables.h"
#include "../2cards.h"
#include "control/3player.h"
#include "../9modifs.h"
#include "build_types.h"
#include "stack.h"
#include <iostream>

std::string Spell::describe() const { return source->describe(); }

void Resolvable::resolve(){
    if(on_resolve) on_resolve->activate(list_of_targets, ctrl, origin->getTarget());
    //the 'if(on_resolve) is there only for two reasons: 1. programming rigorism and 2.Throes of Chaos'
    //really EVERY instant or sorcery will have an on_resolve (except of course Throes of Chaos)
}

//Resolvables are deleted externally by takeopts after having been resolved.
//As a general rule, the one to take the Resolvable out of the stack is the one to destroy it
void Spell::resolve(){
    if(on_resolve) on_resolve->activate(list_of_targets, ctrl, this);
    if(source->getType().underlying == card_type::sorcery) {
        ctrl->putToZone(source, Player::graveyard);
    } else {
        ctrl->myboard.insert(std::move(source), ctrl);
    }
}

void Resolvable::counter(){
	Stack::god->removeFromStack(this);
}

void Spell::counter(){
    gdebug(DBG_TARGETING) << "A Spell called "<< source->getName()<<" is countered! Removing it from stack and deleting it...\n";
    ctrl->putToZone(source, Player::graveyard);
    Stack::god->removeFromStack(this);
}

Spell::Spell(std::unique_ptr<Card> src, Player* ct): Resolvable(ct, src->getEffect()), source(std::move(src)) {}

Resolvable::Resolvable(Player* ct, const Effect_H* tocast, Target* org): Target(description), ctrl(ct){
	//Technically, Resolvables are put on the stack, then targets are chosen.
	// Olympus decided that objects with no targets chosen were PreRes
	//and thus not on the stack. However, it should be displayed somewhere.
	if(org) origin = std::make_unique<Targeter>(org);
	target_flags = target_type::resolvable;
	if(tocast){
		nb_targets = tocast->getNbParams();
		list_of_targets = new Targeter[nb_targets]; //the last being for the origin
		on_resolve = tocast;
		if(nb_targets){
			/*std::string name = "this spell or ability";
			if(org) name = org->get_name();
			Game::god->disp_preRes(tocast, name); */
		}
		const char* params = tocast->getParams();
		for(int i=0; i<nb_targets; i++){
			list_of_targets[i].setTarget( ct->getAgent().chooseTarget(params[i]) );
		}
	} else{ //the resolvable, e.g. a permanent spell, has no on_resolve abilities
		nb_targets = 0;
		list_of_targets = nullptr;
		on_resolve = nullptr;
	}
}

colorId::type Resolvable::getColor() const {
    if(origin) return origin->colorIdentity;
    else return 0; //TODO FEATURE: when a Source dies, its characteristics (color, MV, etc.) is remembered by Resolvables
}

colorId::type Spell::getColor() const {
    return source->getColor();
}

//void PermOption::check_and_pop(int n_of_zone, Player* pl){} //should check whether requirements are still filled. Not implemented yet.