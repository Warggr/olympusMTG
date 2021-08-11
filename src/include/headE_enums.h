#ifndef OLYMPUS_HEAD_E_ENUMS_H
#define OLYMPUS_HEAD_E_ENUMS_H

#include <cstdlib>

using flag_t = unsigned char;

namespace target_type {
    constexpr flag_t damageable = 0x80,
            permanent = 0x40,
            resolvable = 0x20,
            card = 0x10,
            number = 0x08, nonzero = 0x9, nonnegative = 0xa, strictpositive = 0xb, added_mana = 0x0c,
            player = 0x81, creature = 0xc2, planeswalker = 0xc4,
            spell = 0x31, ability = 0x22,
            none = 0x0;
}

enum class object_type { player, permanent, resolvable, card };
enum class permanent_type { land, artifact, planeswalker, creature };
namespace mtg {
    enum class color { white, blue, black, red, green };
    enum class manatype { white, blue, black, red, green, colorless };
}

enum effect_type {
    deal_damage, draw, gain_life, set_life, add_mana, destroy, exile, counter, put_counter, untap, ueot
};

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
    enum basic_type { flagged, basic, creature, planeswalker, instant, sorcery, invalid };
    char legendary : 1, snow : 1;
    char land : 1, artifact : 1, enchantment : 1;
    basic_type underlying : 3;

    constexpr card_type(): legendary(0), snow(0), land(0), artifact(0), enchantment(0), underlying(invalid) {};
    permanent_type toPermType() const {
        switch(underlying) {
            case basic: return permanent_type::land;
            case creature: return permanent_type::creature;
            case planeswalker: return permanent_type::planeswalker;
            case flagged:
                if(land) return permanent_type::land;
                else return permanent_type::artifact;
            default:
                exit(1);
        }
    };
};

enum trigtype {};

#endif //OLYMPUS_HEAD_E_ENUMS_H
