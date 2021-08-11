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

    OptionAction* chooseOpt(bool sorcerySpeed, Player* pl) override;

    void splitDamage(int power, std::list<std::pair<uint8_t, SpecificTargeter<Creature>>>) override;

    std::list<std::unique_ptr<Card>> chooseCardsToKeep(std::list<std::unique_ptr<Card>>& list) override;

    bool keepsHand() override;

    bool chooseAttackers(CollectionTN<Creature>& mycreas, StateTN<Creature>& myattackers) override;

    void chooseBlockers(CollectionTN<Creature>& mycreas, StateTN<Creature>& attackers) override;
};

#endif //OLYMPUS_LOCALAGENT_H
