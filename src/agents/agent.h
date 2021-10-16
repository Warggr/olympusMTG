#ifndef OLYMPUS_AGENT_H
#define OLYMPUS_AGENT_H

#include <iostream>
#include <memory>
#include <utility>
#include <vector>
#include <list>
#include <forward_list>

class Target; class Creature; template<typename T> class SpecificTargeter; class Card;
class Player; class Game;
class OptionAction; class EmptyOption; class SpellOption; class PermOption;
template<typename T> class Y_Hashtable; template<typename T> class StateTN;

#ifdef MOCK_AGENT
enum playerType { MOCK };
#else
enum playerType { LOCAL, /*BOT, NETWORK*/  };
#endif
enum oracle_type { reference, customcard, compiled_customcard };

struct OracleDescr {
    oracle_type type;
    unsigned char nb;
    std::string initializer;
    OracleDescr(oracle_type type, unsigned char nb, std::string initializer):
        type(type), nb(nb), initializer(std::move(initializer)) {};
    OracleDescr(oracle_type type, unsigned char nb, const char* initializer):
            type(type), nb(nb), initializer(initializer) {};

    friend std::ostream& operator<<( std::ostream& output, const OracleDescr& desc );
};

class Viewer;

class Agent {
protected:
    virtual std::vector<OracleDescr> getDeck() = 0;
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

    virtual uptr<OptionAction> chooseOpt(bool sorcerySpeed, Player* pl) = 0;

    virtual uint chooseAmong(std::vector<PermOption*> opts) = 0;
    virtual uint chooseAmong(std::vector<SpellOption*> opts) = 0;

    virtual Target* chooseTarget(char type) = 0;

    virtual void splitDamage(int power, std::list<std::pair<uint8_t, SpecificTargeter<Creature>>>& blockers) = 0;

    virtual bool keepsHand(const fwdlist<uptr<Card>>& cards) = 0;
    virtual std::list<std::unique_ptr<Card>> chooseCardsToKeep(std::list<std::unique_ptr<Card>>& list, unsigned nbToDiscard) = 0;

    virtual bool chooseAttackers(Y_Hashtable<Creature>& mycreas) = 0;
    virtual void chooseBlockers(Y_Hashtable<Creature>& mycreas, StateTN<Creature>& attackers) = 0;
};

class Viewer {
public:
    virtual ~Viewer() = default;
    virtual void connectGame(Game* game) = 0;

    virtual void onDraw(const std::list<uptr<Card>>& cards) = 0;
};

std::unique_ptr<Agent> createAgent(playerType desc);

#endif //OLYMPUS_AGENT_H
