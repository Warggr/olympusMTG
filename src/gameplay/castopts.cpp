#include "oracles/classes/8options.h"
#include "oracles/classes/PermOption.h"
#include "2cards.h"
#include "control/3player.h"
#include <iostream>
#include "resolvables/5resolvables.h"
#include "resolvables/stack.h"

//These functions are somehow ignored by the linker. So they have been defined again in createopts.cpp
bool SpellOption::iscastable(const Player* pl) const {
    if(isLand) return pl->canPlayLand();
    else return pl->manapool >= cost;
}

void SpellOption::cast_opt(Player* pl, uptr<Card>& origin){
    gdebug(DBG_TARGETING) << "CASTING SPELL " << origin->getName() << "\n";
    EmptyOption::payCosts(pl);
    if(isLand) pl->resolvePlayland(std::move(origin));
    else Stack::god->addToStack(std::make_unique<Spell>(std::move(origin), pl));
}

bool PermOption::iscastable(const Player* pl) const {
    return pl->manapool >= cost;
}

void EmptyOption::payCosts(Player *pl) const {
    pl->manapool -= cost;
}

void PermOption::straight_cast(Player* pl){
    Resolvable typecasted(pl, &effects);
    typecasted.resolve();
}

void PermOption::cast_opt(Player* pl, Permanent& origin){
    EmptyOption::payCosts(pl);
    if(tapsymbol) origin.tap();
    Stack::god->addToStack(std::make_unique<Resolvable>(pl, &effects));
}

/*
void Player::check_too_expensive(){
	for(int i=0; i<2; i++){
		myoptions[i*2 + 1] = myoptions[i*2]; //setting all to 'too expensive'
		for(EmptyOption* iter = myoptions[i*2]; iter != 0 && iter != myoptions[i*2 + 2]; iter = iter->next){
			if(iter->cost > possiblepool) break; //options from here on can't possibly be cheap enough
			else if(possiblepool >= iter->cost){ //this one's castable
				if(iter == myoptions[i*2 + 1]){ //first 'too expensive' -> easy, just move the 'too expensive' pointer
					myoptions[i*2 + 1] = iter;
				} else { //in the middle of too expensive ones -> harder
					EmptyOption* i2 = iter->prev;
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

EmptyOption* mergeSortCosts(EmptyOption* start, EmptyOption* end, int nb){
	//std::cout << "----MERGESORT CALL----\n";
	//disp(start, end);
	//std::cout << "\n";
	if(nb==0 || nb == 1) return start;
	EmptyOption* part2 = start;
	for(int i=0; i<nb/2; i++) part2 = part2->next;
	start = mergeSortCosts(start, part2, nb/2);
	part2 = mergeSortCosts(part2, end, nb-nb/2);
	EmptyOption* ret = merge(start, part2, end);
	//std::cout << "mergeSortCosts: returning " << ret->tag << "\n";
	return ret;
}

 //start2 is supposed to have a tail (end), while start1 will be absorbed up to 0 or start2
EmptyOption* merge(EmptyOption* start1, EmptyOption* start2, EmptyOption* end){ //first = cheapest
	//std::cout << "Merging ";
	//disp(start1, start2);
	//std::cout << " with ";
	//disp(start2, end);
	//std::cout << "\n";
	if(!start1 || start1 == start2) return start2;
	if(!start2 || start2 == end) return start1;
	EmptyOption* ret = start1;
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
		EmptyOption* tmp = start1;
		start1 = start2;
		start2 = tmp;
	} //start1 is always the chain we choose from. Start2 is the chain trying to catch up
	int nbloops = 0;
	while(1){
		nbloops++;
		if(start1->next == end){
			//std::cout << "     After " << start1->tag << " comes the end\n";
			EmptyOption* last; for(last = start2; last->next != 0; last = last->next);
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
			EmptyOption* tmp = start1->next; //orphaning start1 offspring
			start1->next = start2;
			start2->prev = start1; //adopting start2 instead
			start1 = start2;
			//std::cout << "   New chain 2 is ";
			for(EmptyOption* iter = start2; iter != 0 && iter != end; iter = iter->next){
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
		for(EmptyOption* iter = ret; iter != start1; iter = iter->next) //std::cout << iter->tag << "-";
		//std::cout << "\n";
		//std::cout << "   Concurrent chain is ";
		for(EmptyOption* iter = start2; iter != 0 && iter != end; iter = iter->next) //std::cout << iter->tag << "-";
		//std::cout << "\n";
		if(nbloops > 100){
			//std::cout << "Error!\n";
			exit(1);
		}
	}
}
*/
