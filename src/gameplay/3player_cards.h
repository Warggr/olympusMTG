#ifndef OLYMPUS_CLASSES_PLAYER_CARDS_H
#define OLYMPUS_CLASSES_PLAYER_CARDS_H

#include "2cards.h"
#include <forward_list>
#include <memory>

class CardZone {
private:
    int size;
    std::forward_list<std::unique_ptr<Card>> cards;

    inline void inc_size(int i){ size += i; };
public:
    CardZone(): size(0) {};
    CardZone(CardZone&& other) noexcept : size(other.size), cards(std::move(other.cards)) {}
    CardZone& operator=(CardZone&& other) = default;

    void shuffle();
    int drawto(CardZone* target, int nb_cards);
    inline void takeonecard(std::unique_ptr<Card> c){ ++size; cards.push_front(std::move(c)); }
    void placeOnBottom(std::unique_ptr<Card> c);

    inline std::unique_ptr<Card> pop_front() { auto f = std::move(cards.front()); cards.pop_front(); size--; return f; };
    inline bool empty(){ return cards.empty(); }
    void revealTopCards(int nb_cards);

//    std::string describe() const; //TODO
};

#endif //OLYMPUS_CLASSES_PLAYER_CARDS_H