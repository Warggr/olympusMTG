#ifndef OLYMPUS_AGENT_NETWORK_H
#define OLYMPUS_AGENT_NETWORK_H

#include "../agent.h"
#include "network/async.h"

class NetworkAgent: public Agent, public Viewer {
    AsyncNetworker networker;
    Game* game;
public:
	NetworkAgent();

	void specificSetup() override;

	std::string getName() override { return networker.getName(); }
	std::string getLogin() override;
	std::unique_ptr<std::istream> getDeckFile() override;
    //virtual EmptyOption* chooseOpt() override { return nullptr; }

	Target* chooseTarget(char type) override;

    void splitDamage(int power, std::list<std::pair<uint8_t, SpecificTargeter<Creature>>>& blockers) override;

    std::list<CardWrapper> chooseCardsToKeep(std::list<CardWrapper>& list, uint nbToDiscard) override;

    bool keepsHand(const std::forward_list<uptr<Card>>& cards) override;

    Option* chooseOpt(bool sorcerySpeed, Player* pl) override;

    bool chooseAttackers(Y_Hashtable<Creature>& mycreas) override;

    void chooseBlockers(Y_Hashtable<Creature>& mycreas, StateTN<Creature>& attackers) override;

    Viewer& getViewer() override { return *this; }

    uint chooseAmong(std::vector<CardOption*> opts) override;
    uint chooseAmong(std::vector<PermOption*> opts) override;

    void connectGame(Game* game) override;
    void onDraw(const std::list<CardWrapper> &cards) override;
    void registerMe(Player* pl) override;
};

#endif //OLYMPUS_AGENT_NETWORK_H
