#include "../HFiles/classes.h"

void Creature::hit(Damageable* tgt){
	tgt->damage(power);
}

void Spell::resolve(){
	if(on_resolve){
		on_resolve->activate(list_of_targets, ctrl); //the 'if(on_resolve) is there only for two reasons: 1. programming rigorism and 2.Throes of Chaos'
	} //really EVERY instant or sorcery will have an on_resolve (except of course Throes of Chaos)
	switch(source->get_type()){
		case 0:
		case 5:
			ctrl->puttozone(source, 1); break;
		case 2: new Artifact(source, ctrl); break;
		case 3: new Planeswalker(source, ctrl); break;
		case 4: new Creature(source, ctrl); break;
	}
}

void Resolvable::resolve(){
	on_resolve->activate(list_of_targets, ctrl);
}

void PlayLand::cast_opt(Player* pl){
	pl->metagame->addtolog("  Land played");
	pl->set_flags(16);
	new Land(source, pl);
	delete this;
}

Land::Land(Card* src, Player* pl): Permanent(src, pl, 0){
	pl->possiblepool += src->get_cost();
	pl->highestpool += src->get_cost();
	pl->check_too_expensive();
}

Permanent::Permanent(Card* src, Player* pl, int nb_zone): flags(1), source(src), ctrl(pl), color(source->get_color()), prev(0) {
	std::string nm = source->get_name();
	for(int i=0; i<30; i++){
		name[i] = nm[i];
	}
	if(!pl->fperm[nb_zone]){
		pl->fperm[nb_zone] = this;
		next = 0;
	}
	else{
		for(Permanent* iter = pl->fperm[nb_zone]; iter!=0; iter=iter->next){
			const char* n = iter->get_name();
			int i;
			for(i = 0; n[i] == nm[i] && n[i] != 0; i++);
			if((n[i] == 0 && nm[i] == 0) || iter->next == 0){
				prev = iter;
				next = iter->next;
				if(next) next->prev = this;
				iter->next = this;
				break;
			}
		}
	}
	src->get_permabs(&first_actab, &nb_actabs);
}

Creature::Creature(Card* src, Player* pl): Permanent(src, pl, 3), Damageable(0), nb_counters(0), nb_blockers(0), assigned_bl(0){
	char* tmp = src->get_flavor_text();
	power = tmp[0];
	toughness = tmp[1];
}

void Card::getpt(int* pow, int* tou){
	*pow = (int) flavor_text[0];
	*tou = (int) flavor_text[1];
}

void Permanent::pop(){
	if(next) next->prev = prev;
	if(prev) prev->next = next;
	else ctrl->fperm[source->get_type()-1] = next;
	prev = 0; next = 0;
}

void Creature::pop(){
	if(next) next->prev = prev;
	if(prev) prev->next = next;
	else{
		if(ctrl->fperm[3] == this) ctrl->fperm[3] = next;
		else ctrl->fperm[4] = next;
	}
	prev = 0; next = 0;
}

void Permanent::destroy(bool forall){
	pop();
	ctrl->puttozone(source, 1);
	if(forall) next->destroy(true);
	delete this;
}

bool Permanent::directactivate(){
	if(nb_actabs == 1 && first_actab[0].get_ismana()){ //is it a mana ability?
		if(first_actab[0].iscastable(ctrl)){ //if it's castable
			if(first_actab[0].get_tapsymbol()){ //tap it, if needed
				if((flags & 1)){
					flags = flags & (~1);
				}
				else return false; //impossible to tap->don't bother with this permanent
			}
			ctrl->manapool = minus(ctrl->manapool, first_actab[0].cost); //pay the mana
			first_actab[0].straight_cast(ctrl); //add the mana!
			return false; //and pass on to sth else
		}
		else return false; //not castable? don't bother
	}
	else return true; //not a mana ability? Return the permanent, takeopts will know what to do
}

void Permanent::activate(){
	if(nb_actabs == 1){
		if(first_actab[0].get_tapsymbol()) flags = flags & (~1);
		ctrl->manapool = minus(ctrl->manapool, first_actab[0].cost);
		first_actab[0].cast_opt(ctrl);
	}
}

void Land::untap(bool forall){
	flags = flags | 1;
	ctrl->possiblepool += source->get_cost();
	if(forall && next) next->untap(true);
}