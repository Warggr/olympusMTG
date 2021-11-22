#ifndef OLYMPUS_PERM_OPTION_H
#define OLYMPUS_PERM_OPTION_H

#include "1effects.h"
#include "8options.h"
#include "Mana/cost.h"

class PermOption: public Option {
private:
    bool tapsymbol, ismanaability, instantspeed;
    Permanent* origin;
    Effect_H effects;
    Cost cost;
public:
    PermOption(): tapsymbol(false), ismanaability(false) {};
    ~PermOption() = default;
    void init(ReaderVisitor& visitor);

    void castOpt(Player* pl) override;
    void straight_cast(Player* pl);

    bool get_tapsymbol() const { return tapsymbol; };
    bool get_ismana() const { return ismanaability; };
    std::string describe() const override;
    std::string describe(const std::string& cardname) const;
    void disp(BasicIO* io) const override;

    bool isCastable(bool sorceryspeed, Player* player) const override;
};

#endif //OLYMPUS_PERM_OPTION_H