#ifndef OLYMPUS_AGENT_H
#define OLYMPUS_AGENT_H

#include "oracles/filereader/filereader.h"
#include <istream>
#include <memory>
#include <utility>
#include <vector>
#include <list>
#include <string_view>

class Target; class Creature; template<typename T> class SpecificTargeter; class Card; class CardWrapper; class CardWrapper;
class Player; class Game; class Gamer;
class Option; class CardOption; class PermOption;
template<typename T> class Y_Hashtable; template<typename T> class StateTN; template<typename T> class PermanentTN;

struct AgentContext;
class Viewer;

class Agent {
protected:
    virtual void specificSetup() = 0;
public:
    std::vector<OracleDescr> descriptors;

    virtual ~Agent() = default;
    virtual Viewer& getViewer() = 0;
    //Makes sure every player is connected and has a deck. May take a long time.
    //Should also make sure that each deck is *valid*. TODO
    void setup() {
        specificSetup(); //Wait for connection or setup
        descriptors = getDeck(); //Checks whether deck is valid (TODO) and puts it into descriptors
    };
    virtual std::string getName() = 0;
    virtual std::string getLogin() = 0;
    virtual uptr<std::istream> getDeckFile() = 0;
    std::vector<OracleDescr> getDeck();

    virtual const Option* chooseOpt(bool sorcerySpeed, const Player* pl) = 0;

    virtual uint chooseAmong(const std::vector<PermOption>& opts) = 0;
    virtual uint chooseAmong(const std::vector<CardOption*>& opts) = 0;

    virtual const Target* chooseTarget(char type) = 0;

    virtual void splitDamage(int power, std::list<std::pair<uint8_t, SpecificTargeter<Creature>>>& blockers) = 0;

    virtual bool keepsHand(const fwdlist<card_ptr>& cards) = 0;
    virtual std::list<CardWrapper> chooseCardsToKeep(std::list<CardWrapper>& list, unsigned nbToDiscard) = 0;

    virtual void chooseAttackers(StateTN<Creature>& mycreas) = 0;
    virtual void chooseBlockers(Y_Hashtable<Creature>& mycreas, StateTN<Creature>& attackers) = 0;
};

class Viewer {
public:
    virtual ~Viewer() = default;
    virtual void connectGame(Game*) {};
    virtual void connectDeck(const Deck&) {};

    virtual void message(std::string_view message) = 0;
    virtual void onDraw(const std::list<CardWrapper>& cards) = 0;
    virtual void registerMe(Gamer* pl) = 0;
};

#endif //OLYMPUS_AGENT_H
