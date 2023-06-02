#ifndef OLYMPUS_AGENT_NETWORK_H
#define OLYMPUS_AGENT_NETWORK_H

#include "../agent.h"

class NetworkManager;
class AsyncNetworker;

class NetworkAgent: public Agent, public Viewer {
    AsyncNetworker& networker;
    Game* game;
    const CardOracle* first_oracle;
    const Card* first_card;
    std::vector<const Permanent*> permanents;
public:
    NetworkAgent(NetworkManager& server);

    void specificSetup() override;

    std::string getName() override;
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
    uint chooseAmong(const std::vector<PermOption>& opts) override;

    void connectGame(Game* game) override;
    void connectDeck(const Deck& deck) override;
    void onDraw(const std::list<CardWrapper>& cards) override;
    void registerMe(Gamer* pl) override;
    void message(std::string_view message) override;
};

#endif //OLYMPUS_AGENT_NETWORK_H
