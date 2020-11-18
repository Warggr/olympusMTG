#include "../HFiles/classes.h"
#include <cstdlib>
#include <ctime>

Card::Card(char n[30], Mana c, char t, Ability* fa, char nb_params, char* params, int nb_act, PermOption* first_act, char* flavor, Card* ptr):
	cost(c), type(t), fab(fa), nb_parameters(nb_params), parameters(params), nb_actabs(nb_act), first_actab(first_act), flavor_text(flavor), next(ptr) {
	for(int i=0; i<30; i++) name[i] = n[i];
	color = 0;
	for(int i=0; i<6; i++){
		if(c & (15 << 4*i)){
			if(color == 0) color = i;
			else color = 6;
		}
	}
	//if(type == 1) color = 0; //lands are colorless but it doesn't matter right now
}

void CardZone::shuffle(){
	srand(time(NULL));
	Card* start = first;
	first = 0;
	for(int i = 0; start->next!=0; i++){
		int r = rand() % (size-i);
		Card* it2 = start;
		if(r==0){
			start = start->next;
			it2->next = first;
			first = it2;
		}
		else{
			for(int j = 0; j < r-1; j++) it2 = it2->next; //it2 shows predecessor of swapped Card
			Card* tmp = it2->next; //tmp shows swapped card
			it2->next = it2->next->next; //removing swapped Card from "rest" string
			tmp->next = first;
			first = tmp;
		}
	}
}

int CardZone::drawto(CardZone* target, int nb_cards){
	if(nb_cards == 0 || first == 0) return 0;
	Card* iter = first;
	int i;
	for(i = 1; i<nb_cards; i++){
		iter = iter->next;
		if(iter->next == 0) break;
	}
	size -= i;
	target->inc_size(i);
	Card* tmp = target->get_first();
	target->set_first(first);
	first = iter->next;
	iter->next = tmp;
	return i;
}

void Player::puttozone(Card* x, char nb_zone){
	x->next = myzones[(int) nb_zone].get_first();
	myzones[(int) nb_zone].set_first(x);
	myzones[(int) nb_zone].inc_size(1);
}