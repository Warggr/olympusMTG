#include "frontend/basicIO.h"
#include "localagent.h"
#include "gameplay/2cards.h"
#include "gameplay/permanents/4permanents.h"
#include "classes/8options.h"
#include "classes/perm_option.h"
#include "control/7game.h"
#include <fstream>

uptr<std::istream> LocalAgent::getDeckFile() {
    uptr<std::istream> fb;
    do {
        fb = std::make_unique<std::ifstream>(
                viewer.frontEnd->getBasicIO()->getTextInput("Name of the deck file?"),
                std::ios::in );
    } while(!*fb);
    return fb;
}

Target* LocalAgent::chooseTarget(char type) {
    return viewer.frontEnd->chooseTarget(type);
}

Option* LocalAgent::chooseOpt(bool sorcerySpeed, Player *pl) {
    (void) pl; //No idea why we need it
    return viewer.frontEnd->chooseOpt(sorcerySpeed);
}

void LocalAgent::splitDamage(int power, std::list<std::pair<uint8_t, SpecificTargeter<Creature>>>& blockers) {
    viewer.frontEnd->splitDamage(power, blockers);
}

std::list<CardWrapper> LocalAgent::chooseCardsToKeep(std::list<CardWrapper>& list, unsigned nbToDiscard) {
    return viewer.frontEnd->chooseCardsToKeep(list, nbToDiscard);
}

bool LocalAgent::keepsHand(const std::forward_list<uptr<Card>>& cards) {
    viewer.showTop(cards, 7);
    return viewer.frontEnd->getBasicIO()->simpleChoice("Keep Hand", "Mulligan");
}

void LocalAgent::chooseAttackers(StateTN<Creature>& attackers) {
    return viewer.frontEnd->getBasicIO()->chooseAttackers(attackers);
}

void LocalAgent::chooseBlockers(Y_Hashtable<Creature>& mycreas, StateTN<Creature>& attackers) {
    return viewer.frontEnd->chooseblockers(mycreas, attackers);
}

uint LocalAgent::chooseAmong(std::vector<PermOption*> opts) {
    return viewer.frontEnd->getBasicIO()->chooseAmong(opts);
}

uint LocalAgent::chooseAmong(std::vector<CardOption *> opts) {
    return viewer.frontEnd->getBasicIO()->chooseAmong(opts);
}

std::string LocalAgent::getName() {
    return viewer.frontEnd->getBasicIO()->getTextInput("Your name:");
}

std::string LocalAgent::getLogin() {
    return viewer.frontEnd->getBasicIO()->getTextInput("IP address:");
}

void LocalViewer::connectGame(Game* game) {
    frontEnd->registerPlayers(game->players);
}
