#include "localagent.h"
#include "gameplay/2cards.h"
#include "oracles/classes/PermOption.h"
#include <fstream>
#include "control/7game.h"

std::vector<OracleDescr> LocalAgent::getDeck() {
    std::vector<OracleDescr> ret;
    std::ifstream fb("decks/deck1.dck", std::ios::in);
    if(!fb) {
        gdebug(DBG_IMPORTANT | DBG_READFILE) << "Error: no deck\n";
        exit(1);
    }
    while(fb.peek() != EOF) {
        char buffer[1024]; int nb;
        fb >> nb >> std::skipws;
        fb.getline(buffer, 1024);
        assert(buffer[0] == ' ');
        ret.emplace_back(buffer[1] == '"' ? customcard : reference,
                         nb, std::string(buffer + 1));
    }
    return ret;
}

Target* LocalAgent::chooseTarget(char type) {
    return viewer.frontEnd.ui.iterate(true, type);
}

uptr<OptionAction> LocalAgent::chooseOpt(bool sorcerySpeed, Player *pl) {
    (void) pl; //No idea why we need it
    return viewer.frontEnd.ui.chooseOpt(sorcerySpeed);
}

void LocalAgent::splitDamage(int power, std::list<std::pair<uint8_t, SpecificTargeter<Creature>>>& blockers) {
    viewer.frontEnd.splitDamage(power, blockers);
}

std::list<std::unique_ptr<Card>> LocalAgent::chooseCardsToKeep(std::list<std::unique_ptr<Card>>& list, unsigned nbToDiscard) {
    return viewer.frontEnd.io.checklist(list, nbToDiscard, nbToDiscard);
}

bool LocalAgent::keepsHand() {
    return viewer.frontEnd.io.simpleChoice("Keep Hand", "Mulligan");
}

bool LocalAgent::chooseAttackers(Y_Hashtable<Creature>& mycreas) {
    return viewer.frontEnd.ui.chooseattackers(mycreas);
}

void LocalAgent::chooseBlockers(Y_Hashtable<Creature>& mycreas, StateTN<Creature>& attackers) {
    return viewer.frontEnd.ui.chooseblockers(mycreas, attackers);
}

uint LocalAgent::chooseAmong(std::vector<PermOption*> opts) {
    return viewer.frontEnd.io.chooseAmong(opts);
}

uint LocalAgent::chooseAmong(std::vector<SpellOption *> opts) {
    return viewer.frontEnd.io.chooseAmong(opts);
}

void LocalViewer::connectGame(Game* game) {
    frontEnd.ui.registerPlayers(game->players);
}
