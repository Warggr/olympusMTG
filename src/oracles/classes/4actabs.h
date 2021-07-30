#ifndef OLYMPUS_PERMOPTION_H
#define OLYMPUS_PERMOPTION_H

#include <string>

class ActAb_H {
    Mana cost;
    Effect_H* effects; //list of targets and abilities
    bool tapsymbol;
    bool ismanaability;
public:
    void fillout(Mana cost, Effect_H* effects, bool tapsymbol, bool ismanaability) {
        this->cost = cost; this->effects = effects; this->tapsymbol = tapsymbol; this->ismanaability = ismanaability;
    }
    void init(ReaderVisitor& visitor);
    std::string describe(const std::string& cardname) const;
};

#endif //OLYMPUS_PERMOPTION_H
