#ifndef OLYMPUS_HEAD_E_ENUMS_H
#define OLYMPUS_HEAD_E_ENUMS_H

#include <string>

namespace target_type {
    constexpr flag_t damageable = 0x80,
            optionable = 0x40,
                card = optionable | 0x1,
            permanent = optionable | 0x20,
            resolvable = 0x10,
            number = 0x08,
                nonzero = number | 0x1, nonnegative = number | 0x2,
                    strictpositive = nonnegative | nonzero,
                added_mana = number | 0x4,
            player = damageable | 0x1, creature = damageable | permanent | 0x2, planeswalker = damageable | permanent | 0x4,
            none = 0x0;
}

inline bool B_is_a_A(flag_t a, flag_t b) { return (a & b)==b; }

namespace colorId {
    struct type {
        char fields;
        constexpr type(char f = 0) : fields(f) {};
        bool operator==(const type& other) const { return other.fields == fields; }
    };

    static constexpr type white{1 << 0}, blue{1 << 1}, black{1 << 2}, red{1 << 3}, green{1 << 4}, colorless {0};
}

enum class object_type { player, permanent, resolvable, card };
enum class permanent_type { land, artifact, planeswalker, creature };
namespace mtg {
    enum class color { white, blue, black, red, green };
    enum manatype { white, blue, black, red, green, colorless };
    static_assert( static_cast<manatype>(color::white) == manatype::white);
}

namespace effect {
    enum type {
        deal_damage, draw, gain_life, set_life, add_mana, destroy, exile, counter, put_counter, untap, ueot
    };
}
using effect_type = effect::type;

namespace target_type {
    extern const flag_t tars1[];
    extern const flag_t tars2[];
    extern const flag_t tars3[];
    extern const flag_t tars4[];
    extern const flag_t tars5[];
    extern const flag_t tars6[];
    extern const flag_t tars7[];
    extern const flag_t tars8[];
    extern const flag_t* target_types[];
    extern const int target_numbers[];
}

struct card_type {
    enum basic_type { flagged, creature, planeswalker, sorcery };
    uchar legendary : 1, snow : 1;
    uchar land : 1, artifact : 1, enchantment : 1;
    uchar shift: 1; //means 'basic' for a land, 'instant' for a sorcery, and undefined for the rest
    basic_type underlying : 2;

    constexpr card_type(): legendary(0), snow(0), land(0), artifact(0), enchantment(0), shift(0), underlying(flagged) {};
    permanent_type toPermType() const {
        switch(underlying) {
            case creature: return permanent_type::creature;
            case planeswalker: return permanent_type::planeswalker;
            case flagged:
                if(land) return permanent_type::land;
                else return permanent_type::artifact;
            default:
                return permanent_type::creature;
        }
    };
    inline bool isInstant() const { return underlying == sorcery and shift == true; }
    std::string toString() const;
};

enum class trig_type {
    etb, ltb, statechange, becomes, attacks, is_damaged
};

namespace zone {
    enum zone {
        hand, graveyard, battlefield, stack, exile, commandzone
    };
    extern const char* descriptions[6];
}

extern const std::string trigger_descriptions[6];
extern const std::string ability_descriptions[12];

namespace targets {
    constexpr unsigned char you = -1
        , origin = -2
        , attached = -3
        ;
}

#endif //OLYMPUS_HEAD_E_ENUMS_H
