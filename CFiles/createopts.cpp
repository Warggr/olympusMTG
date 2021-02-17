#include ".header_link.h"
#include "../HFiles/8options.h"

void PermOption::fillout(Mana c, PreResolvable* preRes, bool ts, bool ismana){
	cost = c; effects = preRes; tapsymbol = ts; ismanaability = ismana;
}

SpellOption::SpellOption(Card* src, Option* next): Option(0, next, src->get_cost()), source(src){
	if(source->get_type() == 0) instantspeed = true;
	if(next) next->prev = this;
}

PlayLand::PlayLand(Card* src, Option* next): SpellOption(src, next){}

void Player::draw(int nb_cards){
	int nb_spells[2]{0};
	SpellOption* first[2]{0}; //first new instant and first new sorcery
	for(int i=0; i<nb_cards; i++){
		if(myzones[0].cards.empty()){ //milled out
			set_flags(4);
			god.game->haswon = true;
			return;
		} else {
			myzones[0].inc_size(-1);
			Card* crd = myzones[0].cards.front();
			myzones[0].cards.pop_front();
			if(crd->get_type() == 1){
				PlayLand* new_playland = new PlayLand(crd, myoptions[LANDOPTS]);
				if(myoptions[LANDOPTS] == 0){
					if(myoptions[LANDOPTS+1] != 0) new_playland->next = myoptions[LANDOPTS+1];
					if(myoptions[LANDOPTS-1] != 0){
						Option* iter;
						for(iter = myoptions[LANDOPTS-1]; iter->next != 0; iter = iter->next);
						iter->next = new_playland;
					}
				}
				myoptions[LANDOPTS] = new_playland;
			}
			else {
				int tempo_type = crd->has_flash() ? 0 : 1;
				first[tempo_type] = new SpellOption(crd, first[tempo_type]);
				nb_spells[tempo_type]++;
			}
		}
	}
	for(int i=1; i>=0; i--) if(first[i]){
		Option* newopts = mergeSortCosts(first[i], 0, nb_spells[i]);
		if(myoptions[i*2 + 1] == 0){
			if(myoptions[i*2] != 0){
				Option* iter;
				for(iter = myoptions[i*2]; iter->next != 0; iter = iter->next);
				iter->next = newopts;
			}
			if(myoptions[i*2+2] != 0){
				Option* iter;
				for(iter = newopts; iter->next != 0; iter = iter->next);
				iter->next = myoptions[i*2+2];
			}
			myoptions[i*2 + 1] = newopts;
		}
		else myoptions[i*2 + 1] = merge(myoptions[i*2 + 1], newopts, myoptions[i*2 + 2]);
	}
	check_too_expensive();
}

void Player::check_too_expensive(){
	for(int i=0; i<2; i++){
		for(Option* iter = myoptions[i*2 + 1]; iter != 0 && iter != myoptions[i*2 + 2]; iter = iter->next){
			if(iter->cost > possiblepool) break; //options from here on can't possibly be cheap enough
			else if(possiblepool >= iter->cost){ //this one's castable
				if(iter == myoptions[i*2 + 1]){ //first 'too expensive' -> easy, just move the 'too expensive' pointer
					myoptions[i*2 + 1] = iter;
				} else { //in the middle of too expensive ones -> harder
					Option* i2 = iter->prev;
					if(iter->prev) iter->prev->next = iter->next;
					if(iter->next) iter->next->prev = iter->prev;
					iter->prev = myoptions[i*2 + 1]->prev; //inserting right before the 'too expensive'
					iter->next = myoptions[i*2 + 1];
					if(iter->prev) iter->prev->next = iter;
					else myoptions[i*2] = iter;
					iter = i2;
				}
			}
		}
	}
}

Option* mergeSortCosts(Option* start, Option* end, int nb){
	if(nb==0 || nb == 1) return start;
	Option* part2 = start;
	for(int i=0; i<nb/2; i++) part2 = part2->next;
	start = mergeSortCosts(start, part2, nb/2);
	part2 = mergeSortCosts(part2, end, nb-nb/2);
	Option* ret = merge(start, part2, end);
	return ret;
}

/*start2 is supposed to have a tail (end), while start1 will be absorbed up to 0 or start2*/
Option* merge(Option* start1, Option* start2, Option* end){ //first = cheapest
	if(!start1) return start2;
	if(!start2) return start1;
	Option* ret = start1;
	//disconnect both chains; s1-x-x-x s2-x-x-...
	if(start2->prev){
		start2->prev->next = 0;
		start2->prev = 0;
	}
	if(start2->cost < start1->cost){
		ret = start2;
		start2->prev = start1->prev;
		Option* tmp = start1;
		start1 = start2;
		start2 = tmp;
	} //start1 is always the chain we choose from. Start2 is the chain trying to catch up
	while(1){
		if(start1->next == end){
			Option* last; for(last = start2; last->next != 0; last = last->next);
			last->next = end;
		}
		if(start1->next == 0 || start1->next == end){
			start1->next = start2;
			start2->prev = start1;
			return ret;
		}

		if(start2->cost < start1->next->cost){ //catch up
			Option* tmp = start1->next; //orphaning start1 offspring
			start1->next = start2;
			start2->prev = start1; //adopting start2 instead
			start1 = start2;
			start2 = tmp; //'start1 offspring' becomes head of concurrent branch
		}
		else start1 = start1->next;
	}
}