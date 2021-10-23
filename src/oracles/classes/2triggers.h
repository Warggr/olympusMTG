#ifndef OLYMPUS_HEADER_TRIGGERS_10_H
#define OLYMPUS_HEADER_TRIGGERS_10_H
//REMINDER: class TriggerEvent is declared in 1general.h header, not here

#include <string>
class Effect_H; class Player; class Target; class ReaderVisitor;

struct TriggerHolder_H {
    trig_type type;
    Effect_H* effects;

    void init(ReaderVisitor& visitor);
    ~TriggerHolder_H() { delete effects; }

    std::string describe(const std::string& name) const;
    static std::string describe(const std::string& name, const Effect_H* effects, trig_type type);
};

/*A loses/gains B life
A wins/loses the game
A creates B tokens (because C)
A gains ability B (cf. Theros Archetypes)
A creates B tokens (because C) "a PINK effect causes YOU to create BEEBLE tokens"
A sacrifices B (because C)
A prevents damage (dealt by B)
A shuffles their library (because C)
A searches their library (because C)
A does special action B, i.e. investigate, explore, clash, scry, crew, regenerate, mutate, on C
A plays a land
A is destroyed (by B) (cf. Karmic Justice)
A is countered (by B) (cf. Lullmage Mentor)
A 's mana pool is emptied of mana B
and last but not least:
you control [no artifacts (cf. covetous dragon), a dwarf (cf. goblins of the flarg)]; this will be the most tricky (perhaps see it as a LTB/ETB trigger?)
*/

#endif //OLYMPUS_HEADER_TRIGGERS_10_H