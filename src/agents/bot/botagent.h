#ifndef OLYMPUS_BOTAGENT_H
#define OLYMPUS_BOTAGENT_H

#include "../agent.h"

class BotAgent: public Agent, public Viewer {
public:
    BotAgent() = default;
    void specificSetup() override {};
    std::string getName() override;
    std::string getLogin() override;

    uptr<std::istream> getDeckFile() override;

    Target* chooseTarget(char type) override;

    Option* chooseOpt(bool sorcerySpeed, Player* pl) override;

    void splitDamage(int power, std::list<std::pair<uint8_t, SpecificTargeter<Creature>>>& blockers) override;

    std::list<CardWrapper> chooseCardsToKeep(std::list<CardWrapper>& list, unsigned nbToDiscard) override;

    bool keepsHand(const std::forward_list<uptr<Card>>& cards) override;

    bool chooseAttackers(Y_Hashtable<Creature>& mycreas) override;

    void chooseBlockers(Y_Hashtable<Creature>& mycreas, StateTN<Creature>& attackers) override;

    uint chooseAmong(std::vector<PermOption*> opts) override;
    uint chooseAmong(std::vector<CardOption*> opts) override;

    Viewer& getViewer() override { return *this; }

    void connectGame(Game* game) override;

    void onDraw(const std::list<CardWrapper>& cards) override;
    void registerMe(Player* pl) override;
};

#endif //OLYMPUS_BOTAGENT_H