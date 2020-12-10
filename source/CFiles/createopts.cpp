#include "../HFiles/olympus_main.h"

void PermOption::fillout(Mana c, PreResolvable* preRes, bool ts, bool ismana){
	cost = c; effects = preRes; tapsymbol = ts, ismanaability = ismana;
}

SpellOption::SpellOption(Card* src, Player* pl, int opttype): Option(0, pl->get_myoptions(opttype), src->get_cost()), source(src) {
	if(source->get_type() == 0) instantspeed = true;
	if(next) next->prev = this;
}

SpellOption::SpellOption(Card* src, Option* next): Option(0, next, src->get_cost()), source(src){
	if(source->get_type() == 0) instantspeed = true;
	if(next) next->prev = this;
}

PlayLand::PlayLand(Card* src, Player* pl): SpellOption(src, pl, LANDOPTS){}

void Player::draw(int nb_cards){
	int nb_spells = 0;
	SpellOption* first_spell = 0;
	for(int i=0; i<nb_cards; i++){
		if(myzones[0].cards.empty()){
			set_flags(4);
			metagame->haswon = true;
			return;
		}
		Card* crd = myzones[0].cards.front();
		myzones[0].cards.pop_front();
		int x = (int) crd->get_type();
		if(x>2) x = 2;
		if(x==1) myoptions[LANDOPTS] = new PlayLand(crd, this);
		else{
			first_spell = new SpellOption(crd, first_spell);
			nb_spells++;
		}
	}
	myoptions[TOOEXPENSIVE] = merge(mergeSortCosts(first_spell, nb_spells), myoptions[TOOEXPENSIVE]);
	myzones[0].inc_size(-nb_cards);
	check_too_expensive();
}

void Player::check_too_expensive(){
	Option* iter = myoptions[TOOEXPENSIVE];
	while(iter!=0){
		if((iter->cost & 0xf) > (possiblepool & 0xf)) break;
		if(less(iter->cost, possiblepool)){
			Option* i2 = iter;
			iter = iter->next;
			i2->pop(TOOEXPENSIVE, this);
			i2->prev = 0;
			if(i2->instantspeed){ //adding an instant
				i2->next = myoptions[INSTANTOPTS];
				myoptions[INSTANTOPTS] = i2;
			}
			else{ //adding a sorcery
				i2->next = myoptions[SORCERYOPTS];
				myoptions[SORCERYOPTS] = i2;
			}
			if(i2->next) i2->next->prev = i2;
		}
		else iter = iter->next;
	}
}

Option* mergeSortCosts(Option* start, int nb){
	if(nb==0) return start;
	if(nb==1){
		start->prev = start->next = 0;
		return start;
	}
	Option* part2 = start;
	for(int i=0; i<nb/2; i++) part2 = part2->next;
	start = mergeSortCosts(start, nb/2);
	part2 = mergeSortCosts(part2, nb-nb/2);
	Option* ret = merge(start, part2);
	return ret;
}

Option* merge(Option* start1, Option* start2){ //first = cheapest
	Option* last = 0;
	Option* first;
	while(start1 && start2){
		if((start1->cost & 0xf) < (start2->cost & 0xf)){
			if(last) last->next = start1;
			else first = start1;
			start1->prev = last;
			last = start1;
			start1 = start1->next;
		}
		else{
			if(last) last->next = start2;
			else first = start2;
			start2->prev = last;
			last = start2;
			start2 = start2->next;
		}
	}
	if(start1){
		if(last){
			last->next = start1;
			start1->prev = last;
		}
		else first = start1;
	}
	else{
		if(last){
			last->next = start2;
			start2->prev = last;
		}
		else first = start2;
	}
	return first;
}