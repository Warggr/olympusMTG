#ifndef OLYMPUS_LIB3_UI_CLI
#define OLYMPUS_LIB3_UI_CLI

#include "../frontend.h"
#include "lib3_nanoIO.h"
#include "headE_enums.h"
#include <forward_list>
#include <memory>

class CliUI: public AbstractFrontEnd {
    NanoIO io;
    Player* pl; //you might need to return the player as a Target*
public:
//    void create(const char* descr);
//    void update(const char* descr);
//    void del(const char* descr);
//    void bulkOp(const char* descr);

    void splitDamage(int power, std::__cxx11::list<std::pair<uint8_t, SpecificTargeter<Creature>>>& blockers) override;

    Option* chooseOpt(bool sorcerySpeed) override;

    Target* chooseTarget(char type) override;

    void registerPlayers(std::list<Player>&) override {};
    void registerMe(Player* pl) override;

    void chooseblockers(Y_Hashtable<Creature>& defenders, StateTN<Creature>& attackers) override;

    void addCards(const std::list<CardWrapper>&) override;

    void disp(fwdlist<uptr<Card>>::const_iterator begin, fwdlist<uptr<Card>>::const_iterator end) override;

    BasicIO* getBasicIO() override { return &io; }

    void list(zone::zone zone);

    Player* getPl() const { return pl; }
};

#endif //OLYMPUS_LIB3_UI_CLI
