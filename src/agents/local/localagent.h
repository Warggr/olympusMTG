#ifndef OLYMPUS_LOCALAGENT_H
#define OLYMPUS_LOCALAGENT_H

#include "../agent.h"
#include "frontend/frontend.h"

class LocalViewer: public Viewer {
    AbstractFrontEnd* frontEnd;
public:
    LocalViewer(): frontEnd(AbstractFrontEnd::factory()) {};
    ~LocalViewer() { delete frontEnd; }
    void connectGame(Game *game) override;
    void onDraw(const std::list<uptr<Card>>& cards) override;
    void showTop(const std::forward_list<uptr<Card>>& cards, uint nb);

    friend class LocalAgent;
};

class LocalAgent: public Agent {
    LocalViewer viewer;
public:
    LocalAgent() = default;
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

    Viewer& getViewer() override { return viewer; }
};

#endif //OLYMPUS_LOCALAGENT_H
