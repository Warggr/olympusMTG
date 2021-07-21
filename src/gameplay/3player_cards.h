#include <forward_list>

class CardZone{
private:
    int size;
    std::forward_list<Card*> cards;

    inline void inc_size(int i){ size += i; };
public:
    CardZone(): size(0), name{0} {};
    ~CardZone();

    void init(std::ifstream& textfile, std::ofstream& binaryOut);
    void init(std::ifstream& bFile);

    void shuffle();
    int drawto(CardZone* target, int nb_cards);
    inline void takeonecard(Card* c){ ++size; cards.push_front(c); };

    void describe(char* tmp) const;
    void disp(const Rect& zone) const;
    inline Card* pop_front() { Card* f = cards.front(); cards.pop_front(); size--; return f; };
    inline bool empty(){ return cards.empty(); }
};
