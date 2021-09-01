#ifndef OLYMPUS_MOCKAGENT_H
#define OLYMPUS_MOCKAGENT_H

#include "../agent.h"
#include "gameplay/2cards.h"
#include "oracles/classes/8options.h"

class MockAgent: public Agent {
public:
    MockAgent() = default;
    void specificSetup() override {};
    std::string getName() override { return "MockAgent"; }

    std::vector<OracleDescr> getDeck() override { return std::vector<OracleDescr>(); }

    Target* chooseTarget(char type) override { (void) type; return nullptr; }

    uptr<OptionAction> chooseOpt(bool sorcerySpeed, Player* pl) override { (void) sorcerySpeed; (void) pl; return nullptr; }

    void splitDamage(int power, std::list<std::pair<uint8_t, SpecificTargeter<Creature>>>& blockers) override {
        (void) power; (void) blockers;
    }

    std::list<std::unique_ptr<Card>> chooseCardsToKeep(std::list<std::unique_ptr<Card>>& list) override {
        std::list<std::unique_ptr<Card>> ret; ret.splice(ret.cend(), list); return ret;
    }

    bool keepsHand() override { return true; }

    bool chooseAttackers(CollectionTN<Creature>& mycreas) override { (void) mycreas; return false; }

    void chooseBlockers(CollectionTN<Creature>& mycreas, StateTN<Creature>& attackers) override { (void) mycreas; (void) attackers; }

    uint chooseAmong(std::vector<PermOption *> opts) override { (void) opts; return 0; }
    uint chooseAmong(std::vector<SpellOption *> opts) override { (void) opts; return 0; }
};


#endif //OLYMPUS_MOCKAGENT_H
