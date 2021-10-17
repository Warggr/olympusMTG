#ifndef OLYMPUS_AGENT_NETWORK_H
#define OLYMPUS_AGENT_NETWORK_H

#include "../agent.h"
#include "network.h"

class NetworkAgent: public Agent, public Networker, public Viewer {
    bool idle; //whether he has been assigned a connection/IP adress yet
    bool unread; //whether any unread messages are in the buffer
    long message_length;
    Game* game;
    std::string name;

    const char* net_receive() override;
public:
	NetworkAgent();

    long receive() override;
	void specificSetup() override;
    void receiveMessage();

    void setName(const char* name);
	std::string getName() override { return name; }
	std::string getLogin() override;
	std::unique_ptr<std::istream> getDeckFile() override;
    //virtual EmptyOption* chooseOpt() override { return nullptr; }
    void setSock(int sock);

	Target* chooseTarget(char type) override;

    void splitDamage(int power, std::list<std::pair<uint8_t, SpecificTargeter<Creature>>>& blockers) override;

    std::list<std::unique_ptr<Card>> chooseCardsToKeep(std::list<std::unique_ptr<Card>>& list, uint nbToDiscard) override;

    bool keepsHand(const std::forward_list<uptr<Card>>& cards) override;

    uptr<OptionAction> chooseOpt(bool sorcerySpeed, Player* pl) override;

    bool chooseAttackers(Y_Hashtable<Creature>& mycreas) override;

    void chooseBlockers(Y_Hashtable<Creature>& mycreas, StateTN<Creature>& attackers) override;

    Viewer& getViewer() override { return *this; }

    uint chooseAmong(std::vector<SpellOption *> opts) override;
    uint chooseAmong(std::vector<PermOption *> opts) override;

    void connectGame(Game *game) override;
    void onDraw(const std::list<std::unique_ptr<Card>> &cards) override;
};

#endif //OLYMPUS_AGENT_NETWORK_H
