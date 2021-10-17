#include "localagent.h"
#include "gameplay/2cards.h"
#include "oracles/classes/perm_option.h"
#include <fstream>
#include <filesystem>
#include "control/7game.h"

uptr<std::istream> LocalAgent::getDeckFile() {
    auto fb = std::make_unique<std::ifstream>(
            viewer.frontEnd->getBasicIO()->getTextInput("Name of the deck file?"),
            std::ios::in
    );
    if(!*fb) {
        gdebug(DBG_IMPORTANT | DBG_READFILE) << "Error: no deck\n";
        exit(1);
    }
    return fb;
}

Target* LocalAgent::chooseTarget(char type) {
    return viewer.frontEnd->chooseTarget(type);
}

uptr<OptionAction> LocalAgent::chooseOpt(bool sorcerySpeed, Player *pl) {
    (void) pl; //No idea why we need it
    return viewer.frontEnd->chooseOpt(sorcerySpeed);
}

void LocalAgent::splitDamage(int power, std::list<std::pair<uint8_t, SpecificTargeter<Creature>>>& blockers) {
    viewer.frontEnd->splitDamage(power, blockers);
}

std::list<std::unique_ptr<Card>> LocalAgent::chooseCardsToKeep(std::list<std::unique_ptr<Card>>& list, unsigned nbToDiscard) {
    return viewer.frontEnd->chooseCardsToKeep(list, nbToDiscard);
}

bool LocalAgent::keepsHand(const std::forward_list<uptr<Card>>& cards) {
    viewer.showTop(cards, 7);
    return viewer.frontEnd->getBasicIO()->simpleChoice("Keep Hand", "Mulligan");
}

bool LocalAgent::chooseAttackers(Y_Hashtable<Creature>& mycreas) {
    return viewer.frontEnd->chooseattackers(mycreas);
}

void LocalAgent::chooseBlockers(Y_Hashtable<Creature>& mycreas, StateTN<Creature>& attackers) {
    return viewer.frontEnd->chooseblockers(mycreas, attackers);
}

uint LocalAgent::chooseAmong(std::vector<PermOption*> opts) {
    return viewer.frontEnd->getBasicIO()->chooseAmong(opts);
}

uint LocalAgent::chooseAmong(std::vector<SpellOption *> opts) {
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
