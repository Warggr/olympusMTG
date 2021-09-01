#include "localagent.h"
#include "gameplay/2cards.h"
#include "oracles/classes/PermOption.h"
#include <fstream>

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
    return frontEnd.ui.iterate(true, type);
}

uptr<OptionAction> LocalAgent::chooseOpt(bool sorcerySpeed, Player *pl) {
    (void) pl; //No idea why we need it
    return frontEnd.ui.chooseOpt(sorcerySpeed);
}

void LocalAgent::splitDamage(int power, std::list<std::pair<uint8_t, SpecificTargeter<Creature>>>& blockers) {
    frontEnd.splitDamage(power, blockers);
}

std::list<std::unique_ptr<Card>> LocalAgent::chooseCardsToKeep(std::list<std::unique_ptr<Card>>& list) {
    return frontEnd.io.checklist(list);
}

bool LocalAgent::keepsHand() {
    return frontEnd.io.simpleChoice("Keep Hand", "Mulligan");
}

bool LocalAgent::chooseAttackers(CollectionTN<Creature>& mycreas) {
    return frontEnd.ui.chooseattackers(mycreas);
}

void LocalAgent::chooseBlockers(CollectionTN<Creature>& mycreas, StateTN<Creature>& attackers) {
    return frontEnd.ui.chooseblockers(mycreas, attackers);
}

uint LocalAgent::chooseAmong(std::vector<PermOption*> opts) {
    return frontEnd.io.chooseAmong(opts);
}

uint LocalAgent::chooseAmong(std::vector<SpellOption *> opts) {
    return frontEnd.io.chooseAmong(opts);
}
