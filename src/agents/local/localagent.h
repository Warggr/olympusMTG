#ifndef OLYMPUS_LOCALAGENT_H
#define OLYMPUS_LOCALAGENT_H

#include "../agent.h"
#include "frontend/frontend.h"

class LocalViewer: public Viewer {
    AbstractFrontEnd* frontEnd;
public:
    LocalViewer(): frontEnd(AbstractFrontEnd::factory()) {};
    ~LocalViewer() { delete frontEnd; }
    void connectGame(Game* game) override;
    void registerMe(Gamer* pl) override;
    void onDraw(const std::list<CardWrapper>& cards) override;
    void message(std::string_view message) override { frontEnd->getBasicIO()->message(message); }
    void showTop(const std::forward_list<card_ptr>& cards, uint nb);

    friend class LocalAgent;
};

class LocalAgent: public Agent {
    LocalViewer viewer;
public:
    LocalAgent() = default;
    void specificSetup() override { viewer.frontEnd->focus(); };
    std::string getName() override;
    std::string getLogin() override;

    uptr<std::istream> getDeckFile() override;

    const Target* chooseTarget(char type) override;

    const Option* chooseOpt(const Option::CastingContext& context) override;

    void splitDamage(int power, std::list<std::pair<uint8_t, SpecificTargeter<Creature>>>& blockers) override;

    std::list<CardWrapper> chooseCardsToKeep(std::list<CardWrapper>& list, unsigned nbToDiscard) override;

    bool keepsHand(const std::forward_list<card_ptr>& cards) override;

    void chooseAttackers(StateTN<Creature>& attackers) override;

    void chooseBlockers(Y_Hashtable<Creature>& mycreas, StateTN<Creature>& attackers) override;

    uint chooseAmong(const std::vector<const Option*>& opts) override;

    Viewer& getViewer() override { return viewer; }
};

#endif //OLYMPUS_LOCALAGENT_H
