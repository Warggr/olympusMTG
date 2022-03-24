#ifndef OLYMPUS_AGENT_NETWORK_H
#define OLYMPUS_AGENT_NETWORK_H

#include "../agent.h"
#include "network/server/async.h"
#include <map>

class NetworkAgent: public Agent, public Viewer {
    AsyncNetworker networker;
    Game* game;
    const CardOracle* first_oracle;
    const Card* first_card;
public:
    NetworkAgent();

    void specificSetup() override;

    std::string getName() override { return networker.getName(); }
    std::string getLogin() override;
    std::unique_ptr<std::istream> getDeckFile() override;
    //virtual EmptyOption* chooseOpt() override { return nullptr; }

    const Target* chooseTarget(char type) override;

    void splitDamage(int power, std::list<std::pair<uint8_t, SpecificTargeter<Creature>>>& blockers) override;

    std::list<CardWrapper> chooseCardsToKeep(std::list<CardWrapper>& list, uint nbToDiscard) override;

    bool keepsHand(const std::forward_list<card_ptr>& cards) override;

    const Option* chooseOpt(bool sorcerySpeed, const Player* pl) override;

    void chooseAttackers(StateTN<Creature>& mycreas) override;

    void chooseBlockers(Y_Hashtable<Creature>& mycreas, StateTN<Creature>& attackers) override;

    Viewer& getViewer() override { return *this; }

    uint chooseAmong(const std::vector<CardOption*>& opts) override;
    uint chooseAmong(const std::vector<PermOption*>& opts) override;

    void connectGame(Game* game) override;
    void connectDeck(const Deck& deck) override;
    void onDraw(const std::list<CardWrapper>& cards) override;
    void registerMe(Player* pl) override;
    void message(const char *message) override;
};

#endif //OLYMPUS_AGENT_NETWORK_H
