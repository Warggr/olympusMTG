#ifndef OLYMPUS_CLASSES_PLAYER_CARDS_H
#define OLYMPUS_CLASSES_PLAYER_CARDS_H

#include "2cards.h"
#include <forward_list>
#include <memory>

class CardZone {
private:
    uint size;
    std::forward_list<card_ptr> cards;

    inline void inc_size(int i){ size += i; };
public:
    CardZone(): size(0) {};
    CardZone(CardZone&& other) noexcept : size(other.size), cards(std::move(other.cards)) {}
    CardZone& operator=(CardZone&& other) = default;
    void init(const Deck& deck) noexcept {
        size = deck.cards.size();
        for(auto& card : deck.cards) cards.push_front(&card);
    }

    void shuffle();
    int drawto(CardZone* target, int nb_cards);
    inline void takeonecard(card_ptr c){ ++size; cards.push_front(move_cardptr(c)); }
    void placeOnBottom(card_ptr c);

    inline card_ptr pop_front() {
        if(empty()) return nullptr;
        auto f = move_cardptr(cards.front()); cards.pop_front(); size--; return f;
    }
    inline bool empty(){ return cards.empty(); }
    void revealTopCards(int nb_cards);

    const std::forward_list<card_ptr>& getCards() const { return cards; }
    uint getSize() const { return size; }

//    std::string describe() const; //TODO
};

#endif //OLYMPUS_CLASSES_PLAYER_CARDS_H
