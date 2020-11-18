#include "../HFiles/classes.h"

void Game::remove_from_stack(Resolvable* rs){
	if(stack == rs) stack = rs->next;
	else{
		Resolvable* iter;
		for(iter = stack; iter->next != rs; iter = iter->next)
			if(iter == 0)
				god.gdebug(DBG_TARGETING | DBG_IMPORTANT) << "Error: countered Resolvable not in stack\n";
		iter->next = rs->next;
	}
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

void SpellOption::cast_opt(Player* pl){ //spell removed from everything by pop, is now reattached to stack or directly executed
	god.gdebug(DBG_TARGETING) << "CASTING SPELL " << source->get_name() << std::endl;
	Spell* typecasted = new Spell(source, pl);
	pl->manapool = minus(pl->manapool, cost);
	pl->metagame->addtostack(typecasted);
	delete this;
}

Spell::Spell(Card* src, Player* ct):
Resolvable(ct, src->get_nb_params(), src->get_ability(), src->get_parameters()), source(src) {
}

Resolvable::Resolvable(Player* ct, int nb_tars, Ability* fab, char* paramtypelist): ctrl(ct), nb_targets((char) nb_tars){
	list_of_targets = new Targeter[nb_tars];
	on_resolve = fab;
	for(int i=0; i<nb_tars; i++){
		ct->metagame->message("Choose a target");
		Player* asker = ct;
		list_of_targets[i].setTarget( iterate(true, &asker, paramtypelist[i]) );
	}
}

void Option::pop(int n_of_zone, Player* pl){
	if(prev) prev->next = next;
	else pl->set_myoptions(n_of_zone, next);
	if(next) next->prev = prev;
}

void Option::check_and_pop(int n_of_zone, Player* pl){ //Default behavior. removes the option from myoptions[n_of_zone]
	pop(n_of_zone, pl);
}

bool SpellOption::iscastable(Player* pl){
	if(less(source->get_cost(), pl->manapool)) return true;
	return false;
}

bool PlayLand::iscastable(Player* pl){
	return !(pl->hasplayedland());
}

bool Option::iscastable(Player* pl){
	return less(cost, pl->manapool);
}

void PermOption::straight_cast(Player* pl){
	Resolvable typecasted(pl, nbtargets, fab, typesoftargets);
	typecasted.resolve();
}

void PermOption::cast_opt(Player* pl){
	Resolvable* typecasted = new Resolvable(pl, nbtargets, fab, typesoftargets);
	pl->metagame->addtostack(typecasted);
}

//void PermOption::check_and_pop(int n_of_zone, Player* pl){} //should check whether requirements are still filled. Not implemented yet.