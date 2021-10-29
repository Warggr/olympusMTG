#include "botagent.h"
#include <fstream>

#include "gameplay/2cards.h"

std::string BotAgent::getName() {
    return { "Hal" };
}

std::string BotAgent::getLogin() {
    return { "127.0.0.1" };
}

uptr<std::istream> BotAgent::getDeckFile() {
    return std::make_unique<std::ifstream>("decks/deck1.dck", std::ios::in);
}

Target *BotAgent::chooseTarget(char type) {
    (void) type; return nullptr;
}

Option * BotAgent::chooseOpt(bool sorcerySpeed, Player *pl) {
    (void) sorcerySpeed; (void) pl;
    return nullptr;
}

void BotAgent::splitDamage(int power, std::list<std::pair<uint8_t, SpecificTargeter<Creature>>>& blockers) {
    (void) power; (void) blockers;
}

std::list<CardWrapper> BotAgent::chooseCardsToKeep(std::list<CardWrapper>& list, unsigned nbToDiscard) {
    (void) list; (void) nbToDiscard;
    return std::list<CardWrapper>();
}

bool BotAgent::keepsHand(const std::forward_list<uptr<Card>>& cards) {
    (void) cards;
    return true;
}

bool BotAgent::chooseAttackers(Y_Hashtable<Creature>& mycreas) {
    (void) mycreas; return false;
}

void BotAgent::chooseBlockers(Y_Hashtable<Creature>& mycreas, StateTN<Creature>& attackers) {
    (void) mycreas; (void) attackers;
}

uint BotAgent::chooseAmong(std::vector<PermOption*> opts) {
    (void) opts; return 0;
}

uint BotAgent::chooseAmong(std::vector<CardOption *> opts) {
    (void) opts;
    return 0;
}

void BotAgent::connectGame(Game* game) {
    (void) game;
}

void BotAgent::onDraw(const std::list<CardWrapper> &cards) {
    (void) cards;
}

void BotAgent::registerMe(Player* pl) {
    (void) pl;
}
