#ifndef OLYMPUS_FRONTEND_H
#define OLYMPUS_FRONTEND_H

#include "nameUI.h"
#include "oracles/classes/card_oracle.h"
#include "basicIO.h"

#include <string>
#include <list>
#include <memory>

class Creature; template<typename T> class Y_Hashtable; template<typename T> class StateTN; class Card;

/* FrontEnd recreates part of the game state on its own.
 * What it has:
 * the printed cards, the changed cards,
 * What it doesn't have:
 * most hidden information
 */

//FrontEnd acts as a gateway between the program code and the local IO/UI.
// Every call to FrontEnd will switch the context and update the AbstractIO::god and AbstractUI::god values.

class AbstractFrontEnd {
    CardOracle* mycards;
public:
    virtual ~AbstractFrontEnd() = default;

    static AbstractFrontEnd* factory();

//    void create(const char* descr);
//    void update(const char* descr);
//    void del(const char* descr);
//    void bulkOp(const char* descr);

    virtual void splitDamage(int power, std::__cxx11::list<std::pair<uint8_t, SpecificTargeter<Creature>>>& blockers) = 0;

    virtual Option* chooseOpt(bool sorcerySpeed) = 0;

    virtual Target* chooseTarget(char type) = 0;

    std::list<uptr<Card>> chooseCardsToKeep(std::list<uptr<Card>>& list, unsigned nbToDiscard);

    virtual bool attackSwitch(int leftY, int rightY, int topZ, int arrowlength) const = 0;

    virtual void registerPlayers(std::list<Player>& players) = 0;

    virtual bool chooseattackers(Y_Hashtable<Creature>& cowards) = 0;
    virtual void chooseblockers(Y_Hashtable<Creature>& defenders, StateTN<Creature>& attackers) = 0;

    virtual void addCards(const std::list<uptr<Card>>& cards) = 0;

    virtual void disp(fwdlist<uptr<Card>>::const_iterator begin, fwdlist<uptr<Card>>::const_iterator end) = 0;

    virtual BasicIO* getBasicIO() = 0;
};

#endif //OLYMPUS_FRONTEND_H
