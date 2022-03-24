#ifndef OLYMPUS_LIB3_UI_CLI
#define OLYMPUS_LIB3_UI_CLI

#include "../frontend.h"
#include "lib3_nanoIO.h"
#include "headE_enums.h"
#include <forward_list>
#include <memory>

class CliUI: public AbstractFrontEnd {
    NanoIO io;
public:
    const Gamer* pl;
    const std::list<const Gamer*>* players;
//    void create(const char* descr);
//    void update(const char* descr);
//    void del(const char* descr);
//    void bulkOp(const char* descr);

    void splitDamage(int power, std::list<std::pair<uint8_t, SpecificTargeter<Creature>>>& blockers) override;

    const Option* chooseOpt(bool sorcerySpeed) override;

    const Target* chooseTarget(char type) override;

    void registerPlayers(const std::list<const Gamer*>& players){ this->players = &players; }
    void registerMe(const Gamer* pl) override;

    void chooseblockers(Y_Hashtable<Creature>& defenders, StateTN<Creature>& attackers) override;

    void addCards(const std::list<CardWrapper>& cards) override;

    void disp(fwdlist<card_ptr>::const_iterator begin, fwdlist<card_ptr>::const_iterator end) override;

    BasicIO* getBasicIO() override { return &io; }

    void list(zone::zone zone);
};

#endif //OLYMPUS_LIB3_UI_CLI
