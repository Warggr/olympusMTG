#ifndef OLYMPUS_FRONTEND_H
#define OLYMPUS_FRONTEND_H

#include "classes/card_oracle.h"
#include "basicIO.h"

#include <string>
#include <list>
#include <memory>

class Creature; template<typename T> class Y_Hashtable; template<typename T> class StateTN; class Card; class CardWrapper;

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

    virtual const Option* chooseOpt(bool sorcerySpeed) = 0;

    virtual const Target* chooseTarget(char type) = 0;

    std::list<CardWrapper> chooseCardsToKeep(std::list<CardWrapper>& list, unsigned nbToDiscard);

    virtual void registerPlayers(const std::list<const Gamer*>& players) = 0;
    virtual void registerMe(const Gamer* pl) = 0;

    virtual void chooseblockers(Y_Hashtable<Creature>& defenders, StateTN<Creature>& attackers) = 0;

    virtual void addCards(const std::list<CardWrapper>& cards) = 0;

    virtual void disp(fwdlist<uptr<Card>>::const_iterator begin, fwdlist<uptr<Card>>::const_iterator end) = 0;

    virtual BasicIO* getBasicIO() = 0;

    virtual void focus() {}; //AllegroIO needs to set something when it's called from a new thread
};

#endif //OLYMPUS_FRONTEND_H
