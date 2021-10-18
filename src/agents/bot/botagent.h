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

    uptr<OptionAction> chooseOpt(bool sorcerySpeed, Player* pl) override;

    void splitDamage(int power, std::list<std::pair<uint8_t, SpecificTargeter<Creature>>>& blockers) override;

    std::list<std::unique_ptr<Card>> chooseCardsToKeep(std::list<std::unique_ptr<Card>>& list, unsigned nbToDiscard) override;

    bool keepsHand(const std::forward_list<uptr<Card>>& cards) override;

    bool chooseAttackers(Y_Hashtable<Creature>& mycreas) override;

    void chooseBlockers(Y_Hashtable<Creature>& mycreas, StateTN<Creature>& attackers) override;

    uint chooseAmong(std::vector<PermOption *> opts) override;
    uint chooseAmong(std::vector<SpellOption *> opts) override;

    Viewer& getViewer() override { return *this; }

    void connectGame(Game *game) override;

    void onDraw(const std::list<std::unique_ptr<Card>> &cards) override;
};

#endif //OLYMPUS_BOTAGENT_H