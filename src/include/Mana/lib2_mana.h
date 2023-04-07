#ifndef OLYMPUS_HEADER_2_MANA_H
#define OLYMPUS_HEADER_2_MANA_H

#include <string>
#include <array>
#include "../headE_enums.h"

using ExplicitMana = std::array<uint, 7>;

class Mana{
private:
    int mana; //0xXcgrbuw1, in inverse order of priority
    struct positions {
        enum pos: char { GENERIC = 0, FIRST_COLOR = 1, WHITE = 1, BLUE, BLACK, RED, GREEN, LAST_COLOR = GREEN, COLORLESS };
    };
public:
    enum xpos : char { FIRST_COLOR = 0, WHITE = 0, BLUE, BLACK, RED, GREEN, LAST_COLOR = GREEN, COLORLESS, GENERIC };
    static_assert((char)xpos::WHITE == (char)mtg::manatype::white);
    static_assert((char)xpos::GREEN == (char)mtg::manatype::green);
    static_assert((char)xpos::COLORLESS == (char)mtg::manatype::colorless);

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
