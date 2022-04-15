#ifndef OLYMPUS_HEADER_2_MANA_H
#define OLYMPUS_HEADER_2_MANA_H

#include <string>
#include <array>
#include "../headE_enums.h"

using ExplicitMana = std::array<uint, 7>;

class Mana{
private:
    int mana; //0xXcgrbuw1, in inverse order of priority
public:
    struct positions {
        static constexpr char GENERIC = 0, FIRST_COLOR = 1, WHITE = 1, BLUE = 2, BLACK = 3, RED = 4, GREEN = 5, LAST_COLOR = 5, COLORLESS = 6;
        static_assert(WHITE == mtg::manatype::white);
        static_assert(GREEN == mtg::manatype::green);
        static_assert(COLORLESS == mtg::manatype::colorless);
    };

    constexpr Mana(): mana(0){};
    constexpr Mana(int i): mana(i){};
    Mana(const char* x);
    std::string m2t() const;
    bool operator==(Mana other) const;
    bool operator>=(Mana pool) const;
    bool operator>(Mana pool) const;
    bool operator<(Mana pool) const;
    void operator-=(Mana cost);
    void operator+=(char color);
    void operator+=(Mana m);
    colorId::type m2color() const;
    inline int m2i() const { return mana; };
    inline uint cmc() const { return mana & 0xf; }
    ExplicitMana m2x() const;

    friend std::ostream& operator<<(std::ostream& os, const Mana& m);
};

#endif //OLYMPUS_HEADER_2_MANA_H