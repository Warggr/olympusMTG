#include "3player_cards.h"
#include "control/3player.h"
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

void CardZone::revealTopCards(int nb_cards) {
    auto iter = cards.begin();
    for(int i=0; i<nb_cards && iter != cards.end(); ++i, ++iter) {
        (*iter)->reveal();
    }
}

void CardZone::placeOnBottom(std::unique_ptr<Card> c) {
    ++size;
    auto iter = cards.before_begin();
    for(; iter._M_next() != cards.end(); iter++);
    cards.emplace_after(iter, std::move(c));
}

void Player::putToZone(std::unique_ptr<Card>& x, enum cardzone nb_zone){
    switch(nb_zone){
        case library_zn: myLibrary.takeonecard(std::move(x)); break;
        case exile_zn: myExile.takeonecard(std::move(x)); break;
        case graveyard_zn: myGraveyard.takeonecard(std::move(x));
    }
}

void Player::draw(int nb_cards) {
    std::list<uptr<Card>> temporaryZone; //according to Magic rules, these cards are already in your hand.
    //We just cache them in a temporary zone and assume nothing happens while they're in the process of being drawn.
    for(int i=0; i<nb_cards; i++) {
        uptr<Card> card = myLibrary.pop_front();
        if(!card){
            milledout = 1;
            //Rule 104.3c: a player who must draw too many cards loses the game the next time a player would receive prio
            return;
        }
        temporaryZone.push_front(std::move(card));
    }
    viewer.onDraw(temporaryZone);
    myHand.splice(myHand.end(), temporaryZone);
}

inline Identifier construct_id_spell(bool is_spell, Card* origin){
    (void) origin; //TODO add info about the card, e.g. CMC
    Identifier ret = (target_type::resolvable) + (is_spell ? 0x20 : 0);
    return ret;
}
