#include "../HFiles/olympus_main.h"

Resolvable* Game::popfromstack(){
	if(stack.empty()) return 0;
	auto ret = stack.front();
	stack.pop_front();
	return ret;
}

void Game::remove_from_stack(Resolvable* rs){
	stack.remove(rs);
}

void Spell::counter(Game* metagame){
	god.gdebug(DBG_TARGETING) << "A Spell called "<< source->get_name()<<" is countered! Removing it from stack and deleting it...\n";
	ctrl->puttozone(source, 0);
	metagame->remove_from_stack(this);
	delete this;
}

void Resolvable::counter(Game* metagame){
	metagame->remove_from_stack(this);
	delete this;
}

Resolvable* SpellOption::cast_opt(Player* pl){ //spell removed from everything by pop, is now reattached to stack or directly executed
	god.gdebug(DBG_TARGETING) << "CASTING SPELL " << source->get_name() << std::endl;
	Resolvable* typecasted = new Spell(source, pl);
	pl->manapool -= cost;
	delete this;
	return typecasted;
}

Spell::Spell(Card* src, Player* ct):
Resolvable(ct, src->get_preRes()), source(src) {
}

Resolvable::Resolvable(Player* ct, const PreResolvable* tocast, Target* org): ctrl(ct){
	//Technically, Resolvables are put on the stack, then targets are chosen. Olympus decided that objects with no targets chosen were PreRes
	//and thus not on the stack. However, it should be displayed somewhere.
	origin = std::make_unique<Targeter>(org);
	target_flags = 0x20;
	if(tocast){
		nb_targets = tocast->getNbParams();
		list_of_targets = new Targeter[nb_targets]; //the last being for the origin
		on_resolve = tocast->getFab();
		if(nb_targets){
			std::string name = "this spell or ability";
			if(org) name = org->get_name();
			god.game->disp_preRes(tocast, name);
		}
		const char* params = tocast->getParams();
		for(int i=0; i<nb_targets; i++){
			god.myIO->message("Choose a target");
			Player* asker = ct;
			list_of_targets[i].setTarget( ct->iterate(true, &asker, params[i]) );
		}
	}
	else{ //the resolvable, e.g. a permanent spell, has no on_resolve abilities
		nb_targets = 0;
		list_of_targets = 0;
		on_resolve = 0;
	}
}

void Option::pop(int n_of_zone, Player* pl) const{
	if(prev) prev->next = next;
	else pl->set_myoptions(n_of_zone, next);
	if(next) next->prev = prev;
}

void Option::check_and_pop(int n_of_zone, Player* pl){ //Default behavior. removes the option from myoptions[n_of_zone]
	pop(n_of_zone, pl);
}

bool SpellOption::iscastable(const Player* pl) const{
	if(pl->manapool >= source->get_cost()) return true;
	return false;
}

bool PlayLand::iscastable(const Player* pl) const{
	return !(pl->hasplayedland());
}

bool Option::iscastable(const Player* pl) const{
	return pl->manapool >= cost;
}

void PermOption::straight_cast(Player* pl){
	Resolvable typecasted(pl, effects);
	typecasted.resolve();
}

Resolvable* PermOption::cast_opt(Player* pl){
	auto typecasted = new Resolvable(pl, effects);
	return typecasted;
}

//void PermOption::check_and_pop(int n_of_zone, Player* pl){} //should check whether requirements are still filled. Not implemented yet.