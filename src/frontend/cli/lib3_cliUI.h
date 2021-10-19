#ifndef OLYMPUS_LIB3_UI_CLI
#define OLYMPUS_LIB3_UI_CLI

#include "../frontend.h"
#include "lib3_nanoIO.h"
#include <forward_list>
#include <memory>

class CliUI: public AbstractFrontEnd {
    NanoIO io;
    const std::list<Player>* the_players {nullptr};
public:
//    void create(const char* descr);
//    void update(const char* descr);
//    void del(const char* descr);
//    void bulkOp(const char* descr);

    void splitDamage(int power, std::__cxx11::list<std::pair<uint8_t, SpecificTargeter<Creature>>>& blockers) override;

    uptr<Option> chooseOpt(bool sorcerySpeed) override;

    Target* chooseTarget(char type) override;

    bool attackSwitch(int leftY, int rightY, int topZ, int arrowlength) const override;

    void registerPlayers(std::list<Player>& players) override;

    bool chooseattackers(Y_Hashtable<Creature>& cowards) override;
    void chooseblockers(Y_Hashtable<Creature>& defenders, StateTN<Creature>& attackers) override;

    void addCards(const std::list<uptr<Card>>&) override;

    void disp(fwdlist<uptr<Card>>::const_iterator begin, fwdlist<uptr<Card>>::const_iterator end) override;

    BasicIO* getBasicIO() override { return &io; }
};

#endif //OLYMPUS_LIB3_UI_CLI
