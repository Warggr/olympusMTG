#ifndef OLYMPUS_AGENT_H
#define OLYMPUS_AGENT_H

#include <iostream>
#include <memory>
#include <utility>
#include <vector>
#include <list>

class Target; class Creature; template<typename T> class SpecificTargeter; class Card; class OptionAction; class Player;
template<typename T> class CollectionTN; template<typename T> class StateTN;

enum playerType { LOCAL, /*BOT, NETWORK*/  };
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

class Agent {
protected:
    virtual std::vector<OracleDescr> getDeck() = 0;
    virtual void specificSetup() = 0;
public:
    std::vector<OracleDescr> descriptors;

    virtual ~Agent() = default;
    void setup() {
        specificSetup();
        descriptors = getDeck();
        for(auto& desc : descriptors) std::cout << desc;
    }; //Makes sure every player is connected and has a deck. May take a long time.
    //Should also make sure that each deck is *valid*. TODO
    virtual std::string getName() = 0;

    virtual OptionAction* chooseOpt(bool sorcerySpeed, Player* pl) = 0;

    virtual Target* chooseTarget(char type) = 0;

    virtual void splitDamage(int power, std::list<std::pair<uint8_t, SpecificTargeter<Creature>>>) = 0;

    virtual bool keepsHand() = 0;

    virtual std::list<std::unique_ptr<Card>> chooseCardsToKeep(std::list<std::unique_ptr<Card>>& list) = 0;

    virtual bool chooseAttackers(CollectionTN<Creature>& mycreas, StateTN<Creature>& myattackers) = 0;

    virtual void chooseBlockers(CollectionTN<Creature>& mycreas, StateTN<Creature>& attackers) = 0;
};

std::unique_ptr<Agent> createAgent(playerType desc);

#endif //OLYMPUS_AGENT_H
