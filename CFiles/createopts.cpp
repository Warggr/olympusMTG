#include ".header_link.h"
#include "../HFiles/8options.h"
#include <iostream>

void PermOption::fillout(Mana c, PreResolvable* preRes, bool ts, bool ismana){
	cost = c; effects = preRes; tapsymbol = ts; ismanaability = ismana;
}

SpellOption::SpellOption(Card* src, Option* next): Option(0, next, src->get_cost()), source(src){
	if(source->get_type() == 0) instantspeed = true;
	if(next){
		prev = next->prev;
		if(prev) prev->next = this;
		next->prev = this;
	}
}

PlayLand::PlayLand(Card* src, Option* next): SpellOption(src, next){};

void Player::draw(int nb_cards){
	//verify_chain_integrity(myoptions);
	int nb_spells[2]{0};
	Option* last[2]{0};
	for(int i=0; i<nb_cards; i++){
		if(myzones[0].cards.empty()){ //milled out
			set_flags(4);
			god.game->haswon = true;
			return;
		} else { //in a first step, we add options to the "cheap" subchain
			myzones[0].inc_size(-1);
			Card* crd = myzones[0].cards.front();
			myzones[0].cards.pop_front();
			int zone = (crd->get_type() == 1) ? LANDOPTS : (crd->has_flash()) ? 0 : 2;
			Option* next = next_in_chain(myoptions, zone);
			Option* insert;
			if(crd->get_type() == 1){
				insert = new PlayLand(crd, next);
			}
			else {
				insert = new SpellOption(crd, next);
				if(nb_spells[zone/2] == 0){
					//std::cout << "last[" << zone/2 << "] = " << insert->tag << "\n";
					last[zone/2] = insert;
				}
				nb_spells[zone/2]++;
			}
			if(next == 0){
				insert->prev = myoptions[NBMYOPTS];
				if(insert->prev) insert->prev->next = insert;
				myoptions[NBMYOPTS] = insert;
			}
			myoptions[zone] = insert;
		}
		//verify_chain_integrity(myoptions);
	}
	for(int i=0; i<2; i++){
		if(last[i] != 0){ //if options were added at all to this subchain
			myoptions[i*2+1] = myoptions[i*2]; //reset the "expensive" pointer to "cheap"
			//std::cout << "\n---RECENTLY ADDED----\n";
			Option* end_of_chain = last[i]->next;
			myoptions[i*2] = mergeSortCosts(myoptions[i*2], end_of_chain, nb_spells[i]); //sorting recently added opts by mana cost
			//std::cout << "---REMERGING----\n";
			myoptions[i*2] = merge(myoptions[i*2], end_of_chain, myoptions[i*2 + 2]); //remerging these options with the old ones
			//we could've used mergeSort really, but we know the lower half is already sorted
			//std::cout << "Setting myopts[" << i*2 << "] to " << myoptions[i*2]->tag << "\n";
		}
	}
	while(myoptions[NBMYOPTS]->next != 0) myoptions[NBMYOPTS] = myoptions[NBMYOPTS]->next; //merging stuff -> last might've been changed
	//verify_chain_integrity(myoptions);
	check_too_expensive();
	//verify_chain_integrity(myoptions);
}

void Player::check_too_expensive(){
	for(int i=0; i<2; i++){
		myoptions[i*2 + 1] = myoptions[i*2]; //setting all to 'too expensive'
		for(Option* iter = myoptions[i*2]; iter != 0 && iter != myoptions[i*2 + 2]; iter = iter->next){
			if(iter->cost > possiblepool) break; //options from here on can't possibly be cheap enough
			else if(possiblepool >= iter->cost){ //this one's castable
				if(iter == myoptions[i*2 + 1]){ //first 'too expensive' -> easy, just move the 'too expensive' pointer
					myoptions[i*2 + 1] = iter;
				} else { //in the middle of too expensive ones -> harder
					Option* i2 = iter->prev;
					//disconnecting
					iter->prev->next = iter->next; //iter is 'in the middle' of sth, so it must have a prev
					if(iter->next) iter->next->prev = iter->prev;
					else myoptions[NBMYOPTS] = iter->prev;

					iter->prev = myoptions[i*2 + 1]->prev; //inserting right before the 'too expensive'
					iter->next = myoptions[i*2 + 1];
					if(iter->prev) iter->prev->next = iter;
					iter->next->prev = iter;

					if(myoptions[i*2 + 1] == myoptions[i*2]) myoptions[i*2] = iter;
					
					iter = i2;
				}
			}
		}
	}
}

Option* mergeSortCosts(Option* start, Option* end, int nb){
	//std::cout << "----MERGESORT CALL----\n";
	//disp(start, end);
	//std::cout << "\n";
	if(nb==0 || nb == 1) return start;
	Option* part2 = start;
	for(int i=0; i<nb/2; i++) part2 = part2->next;
	start = mergeSortCosts(start, part2, nb/2);
	part2 = mergeSortCosts(part2, end, nb-nb/2);
	Option* ret = merge(start, part2, end);
	//std::cout << "mergeSortCosts: returning " << ret->tag << "\n";
	return ret;
}

/*start2 is supposed to have a tail (end), while start1 will be absorbed up to 0 or start2*/
Option* merge(Option* start1, Option* start2, Option* end){ //first = cheapest
	//std::cout << "Merging ";
	//disp(start1, start2);
	//std::cout << " with ";
	//disp(start2, end);
	//std::cout << "\n";
	if(!start1 || start1 == start2) return start2;
	if(!start2 || start2 == end) return start1;
	Option* ret = start1;
	//disconnect both chains; s1-x-x-x s2-x-x-...
	if(start2->prev){
		//std::cout << "   disconnect " << start2->prev->tag << " from " << start2->tag << "\n";
		start2->prev->next = 0;
		start2->prev = 0;
	}
	if(start2->cost < start1->cost){
		//std::cout << "   Swap chain 2 for chain 1\n";
		ret = start2;
		start2->prev = start1->prev;
		if(start2->prev) start2->prev->next = start2;
		Option* tmp = start1;
		start1 = start2;
		start2 = tmp;
	} //start1 is always the chain we choose from. Start2 is the chain trying to catch up
	int nbloops = 0;
	while(1){
		nbloops++;
		if(start1->next == end){
			//std::cout << "     After " << start1->tag << " comes the end\n";
			Option* last; for(last = start2; last->next != 0; last = last->next);
			//std::cout << "     Last found to be " << last->tag << "\n";
			last->next = end;
			if(end) end->prev = last;
		}
		if(start1->next == 0 || start1->next == end){
			start1->next = start2;
			start2->prev = start1;
			//std::cout << "  Joining " << start1->tag << " and " << start2->tag << " and returning " << ret->tag << "\n";
			//disp(ret, end);
			//std::cout << "\n";
			return ret;
		}

		if(start2->cost < start1->next->cost){ //catch up
			//std::cout << "   String 2 catched up with " << start2->tag << "!\n";
			Option* tmp = start1->next; //orphaning start1 offspring
			start1->next = start2;
			start2->prev = start1; //adopting start2 instead
			start1 = start2;
			//std::cout << "   New chain 2 is ";
			for(Option* iter = start2; iter != 0 && iter != end; iter = iter->next){
				//std::cout << iter->tag << "-";
				if(iter->next){
					if(iter == iter->next) break;
					if(iter == iter->next->next) break;
				}
			}
			//std::cout << "\n";
			start2 = tmp; //'start1 offspring' becomes head of concurrent branch
		}
		else start1 = start1->next;

		//std::cout << "   Main chain is ";
		for(Option* iter = ret; iter != start1; iter = iter->next) //std::cout << iter->tag << "-";
		//std::cout << "\n";
		//std::cout << "   Concurrent chain is ";
		for(Option* iter = start2; iter != 0 && iter != end; iter = iter->next) //std::cout << iter->tag << "-";
		//std::cout << "\n";
		if(nbloops > 100){
			//std::cout << "Error!\n";
			exit(1);
		}
	}
}

Option* next_in_chain(Option** myoptions, int nb_zone){
	for(nb_zone = nb_zone; nb_zone != NBMYOPTS; nb_zone++){
		if(myoptions[nb_zone] != 0) return myoptions[nb_zone];
	}
	return 0;
}