#include <forward_list>

#include "2cards.h"
#define uptr std::unique_ptr
#include <forward_list>
#include <memory>

class CardZone{
private:
    int size;
    std::forward_list<std::unique_ptr<Card>> cards;

    inline void inc_size(int i){ size += i; };
public:
    CardZone(): size(0) {};
    CardZone(CardZone&& other) noexcept : size(other.size), cards(std::move(other.cards)) {}

    void shuffle();
    int drawto(CardZone* target, int nb_cards);
    inline void takeonecard(std::unique_ptr<Card> c){ ++size; cards.push_front(std::move(c)); };

    inline std::unique_ptr<Card> pop_front() { auto f = std::move(cards.front()); cards.pop_front(); size--; return f; };
    inline bool empty(){ return cards.empty(); }

    std::string describe() const;
    void disp(const Rect& zone) const;
};
