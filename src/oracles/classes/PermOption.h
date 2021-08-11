#ifndef OLYMPUS_PERMOPTION_H
#define OLYMPUS_PERMOPTION_H

#include "1effects.h"
#include "8options.h"

class PermOption: public Option<Permanent> {
private:
    bool tapsymbol, ismanaability;
public:
    PermOption(): Option(0), tapsymbol(false), ismanaability(false) {};
    ~PermOption();
    void init(ReaderVisitor& visitor);

    void cast_opt(Player* pl, Permanent& origin) override;
    void straight_cast(Player* pl);

    bool get_tapsymbol() const { return tapsymbol; };
    bool get_ismana() const { return ismanaability; };
    std::string describe(const std::string& cardname) const;
//    void disp(int y, int z, int width, bool highlight, bool castable) const override;

    void fillout(Mana c, Effect_H *preRes, bool ts, bool ismana);

    bool iscastable(const Player *pl) const override;
};

#endif //OLYMPUS_PERMOPTION_H
