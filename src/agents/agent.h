#ifndef OLYMPUS_AGENT_H
#define OLYMPUS_AGENT_H

#include <istream>
#include <memory>
#include <utility>
#include <vector>
#include <list>
#include <forward_list>

class Target; class Creature; template<typename T> class SpecificTargeter; class Card;
class Player; class Game;
class Option; class CardOption; class PermOption;
template<typename T> class Y_Hashtable; template<typename T> class StateTN;

#define ENABLE_IF_LOCAL(x)
#define ENABLE_IF_NETWORK(x)
#define ENABLE_IF_BOT(x)
#define ENABLE_IF_MOCK(x)

enum playerType {
#ifdef MOCK_AGENT
#undef ENABLE_IF_MOCK
#define ENABLE_IF_MOCK(x) x
    MOCK,
#else
#ifndef NO_LOCAL_AGENT
#undef ENABLE_IF_LOCAL
#define ENABLE_IF_LOCAL(x) x
    LOCAL,
#endif
#ifndef NO_BOT_AGENT
#undef ENABLE_IF_BOT
#define ENABLE_IF_BOT(x) x
    BOT,
#endif
#ifndef NO_NETWORK_AGENT
#undef ENABLE_IF_NETWORK
#define ENABLE_IF_NETWORK(x) x
    NETWORK,
#endif
#endif
};

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
    virtual void specificSetup() = 0;
public:
    std::vector<OracleDescr> descriptors;

    static uptr<Agent> factory(playerType desc);

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

    virtual Option* chooseOpt(bool sorcerySpeed, Player* pl) = 0;

    virtual uint chooseAmong(std::vector<PermOption*> opts) = 0;
    virtual uint chooseAmong(std::vector<CardOption*> opts) = 0;

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
    virtual void registerMe(Player* pl) = 0;
};

#endif //OLYMPUS_AGENT_H
