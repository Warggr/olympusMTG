#ifndef OLYMPUS_PERM_OPTION_H
#define OLYMPUS_PERM_OPTION_H

#include "1effects.h"
#include "8options.h"

class PermOption: public Option {
private:
    bool tapsymbol, ismanaability;
public:
    PermOption(): Option(0), tapsymbol(false), ismanaability(false) {};
    ~PermOption();
    void init(ReaderVisitor& visitor);

    void cast_opt(Player* pl, Permanent& origin);
    void straight_cast(Player* pl);

    bool get_tapsymbol() const { return tapsymbol; };
    bool get_ismana() const { return ismanaability; };
    std::string describe(const std::string& cardname) const;
//    void disp(int y, int z, int width, bool highlight, bool castable) const override;

    bool iscastable(const Player *pl) const override;
};

class PermOptionAction: public OptionAction {
    PermOption& option;
    Permanent& perm;
public:
    PermOptionAction(PermOption& option, Permanent& origin): option(option), perm(origin) {}
    void cast_opt(Player* pl) override {
        option.cast_opt(pl, perm);
    }
};

#endif //OLYMPUS_PERM_OPTION_H
