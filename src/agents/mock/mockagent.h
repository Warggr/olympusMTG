#ifndef OLYMPUS_MOCKAGENT_H
#define OLYMPUS_MOCKAGENT_H

#include "../agent.h"
#include "gameplay/2cards.h"
#include "classes/8options.h"
#include <fstream>

class MockAgent: public Agent, public Viewer {
public:
    MockAgent() = default;
    void specificSetup() override {};
    std::string getName() override { return "MockAgent"; }
    std::string getLogin() override { return "127.0.0.1"; }

    uptr<std::istream> getDeckFile() override {
        return std::make_unique<std::ifstream>("decks/syntax_guide.txt", std::ios::in);
    }

    Target* chooseTarget(char type) override { (void) type; return nullptr; }

    Option* chooseOpt(bool sorcerySpeed, const Player* pl) override { (void) sorcerySpeed; (void) pl; return nullptr; }

    void splitDamage(int power, std::list<std::pair<uint8_t, SpecificTargeter<Creature>>>& blockers) override {
        (void) power; (void) blockers;
    }

    std::list<CardWrapper> chooseCardsToKeep(std::list<CardWrapper>& list, unsigned nbToDiscard) override {
        (void) nbToDiscard; std::list<CardWrapper> ret; ret.splice(ret.cend(), list); return ret;
    }

    bool keepsHand(const fwdlist<card_ptr>&) override { return true; }

    void chooseAttackers(StateTN<Creature>& attackers) override { (void) attackers; }

    void chooseBlockers(Y_Hashtable<Creature>& mycreas, StateTN<Creature>& attackers) override {
        (void) mycreas; (void) attackers;
    }

    uint chooseAmong(const std::vector<PermOption*>& opts) override { (void) opts; return 0; }
    uint chooseAmong(const std::vector<CardOption*>& opts) override { (void) opts; return 0; }

    Viewer& getViewer() override { return *this; }

    void connectGame(Game* game) override { (void) game; }

    void onDraw(const std::list<CardWrapper> &cards) override { (void) cards; }
    void registerMe(Gamer* pl) override { (void) pl; }
    void message(std::string_view) override {};
};

#endif //OLYMPUS_MOCKAGENT_H
