#ifndef OLYMPUS_LOCALAGENT_H
#define OLYMPUS_LOCALAGENT_H

#include "../agent.h"
#include "frontend/frontend.h"

class LocalAgent: public Agent {
    FrontEnd frontEnd;
public:
    LocalAgent() = default;
    void specificSetup() override {};
    std::string getName() override { return frontEnd.getName(); }

    std::vector<OracleDescr> getDeck() override;

    Target* chooseTarget(char type) override;

    uptr<OptionAction> chooseOpt(bool sorcerySpeed, Player* pl) override;

    void splitDamage(int power, std::list<std::pair<uint8_t, SpecificTargeter<Creature>>>& blockers) override;

    std::list<std::unique_ptr<Card>> chooseCardsToKeep(std::list<std::unique_ptr<Card>>& list) override;

    bool keepsHand() override;

    bool chooseAttackers(Y_Hashtable<Creature>& mycreas) override;

    void chooseBlockers(Y_Hashtable<Creature>& mycreas, StateTN<Creature>& attackers) override;

    uint chooseAmong(std::vector<PermOption *> opts) override;
    uint chooseAmong(std::vector<SpellOption *> opts) override;
};

#endif //OLYMPUS_LOCALAGENT_H
