#ifndef OLYMPUS_GUI_FRONTEND_H
#define OLYMPUS_GUI_FRONTEND_H

#include "../frontend.h"
#include "io/6abstractIO.h"
#include "windows/argus.h"

struct GraphicalUI: public AbstractFrontEnd {
    AbstractIO* io;
    Screen screen;

    GraphicalUI();

    void registerPlayers(std::list<Player>& players) override;
    void addCards(const std::list<uptr<Card>>& cards) override;

    void splitDamage(int power, std::__cxx11::list<std::pair<uint8_t, SpecificTargeter<Creature>>>& blockers) override;

    uptr<Option> chooseOpt(bool sorcerySpeed) override;

    bool chooseattackers(Y_Hashtable<Creature>& cowards) override;
    void chooseblockers(Y_Hashtable<Creature>& defenders, StateTN<Creature>& attackers) override;
    Creature* blockerSwitch(const Creature& blocker, int blockerIndex, StateTN<Creature>& attackers);

   Target* chooseTarget(char type) override;

   bool attackSwitch(int leftY, int rightY, int topZ, int arrowlength) const override;

   void disp(fwdlist<uptr<Card>>::const_iterator begin, fwdlist<uptr<Card>>::const_iterator end) override;

   BasicIO* getBasicIO() override { return io; }
};

namespace AbstractUI {
    constexpr int ELTYPE_STACK = 0, ELTYPE_LOGBOOK = 1, ELTYPE_PERMANENTS = 2;
}

#endif //OLYMPUS_GUI_FRONTEND_H
