#include "3player_cards.h"
#include <random>

void CardZone::shuffle(){
	std::default_random_engine generator;
	std::forward_list<uptr<Card>> newlist;
	for(int i=0; i<size; i++){
        std::uniform_int_distribution<int> distribution(0, size - i - 1);
		int r = distribution(generator) % (size-i);
		
		auto start = cards.before_begin();
		for(int j = 0; j < r; j++) start++; //start shows predecessor of swapped Card
		cards.splice_after(newlist.before_begin(), newlist, start);
	}
	cards = std::move(newlist);
}

int CardZone::drawto(CardZone* target, int nb_cards){
	if(nb_cards == 0 || cards.empty()) return 0;
	auto iter = cards.begin();
	int i;
	for(i=0; i<nb_cards; i++){
		iter++;
		if(iter == cards.end()) break;
	}
	size -= i;
	target->inc_size(i);
	cards.splice_after(target->cards.before_begin(), target->cards, cards.before_begin(), iter);
	return i;
}

void Player::puttozone(Card* x, char nb_zone){
	myzones[(int) nb_zone].takeonecard(x);
}
