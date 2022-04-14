#ifndef OLYMPUS_PERM_OPTION_H
#define OLYMPUS_PERM_OPTION_H

#include "1effects.h"
#include "8options.h"
#include "Mana/cost.h"

class PermOption_H {
private:
    bool tapsymbol, ismanaability, instantspeed;
    Effect_H effects;
    Cost cost;
public:
    PermOption_H(): tapsymbol(false), ismanaability(false) {};

    std::string describe(const std::string& cardname) const;

    bool get_tapsymbol() const { return tapsymbol; };
    bool get_ismana() const { return ismanaability; };

    template<bool read> friend void visit(ConstHost<PermOption_H, read>&, Visitor<read>&);
    friend class PermOption;
};

class PermOption : public Option {
    Permanent* origin;
    const PermOption_H& content;
public:
    PermOption(const PermOption_H& content): content(content) {};
    void init(Permanent* origin) { this->origin = origin; } 
    bool isCastable(bool sorceryspeed, const Player* player) const override;
    bool castOpt(Player* pl) override;
    void straight_cast(Player* pl);
    
    std::string describe() const override;
    void disp(BasicIO* io) const override;
};

#endif //OLYMPUS_PERM_OPTION_H
