#include "localagent.h"
#include "gameplay/2cards.h"
#include "oracles/classes/PermOption.h"
#include <fstream>

std::vector<OracleDescr> LocalAgent::getDeck() {
    std::vector<OracleDescr> ret;
    std::ifstream fb("material/decks/deck1.dck", std::ios::in);
    while(fb.peek() != EOF) {
        char buffer[1024]; int nb;
        fb >> nb >> std::skipws;
        fb.getline(buffer, 1024);
        ret.emplace_back(customcard, nb, std::string(buffer));
    }
    return ret;
}

Target* LocalAgent::chooseTarget(char type) {
    return frontEnd.ui.iterate(true, type);
}

uptr<OptionAction> LocalAgent::chooseOpt(bool sorcerySpeed, Player *pl) {
    return frontEnd.ui.chooseOpt(sorcerySpeed, pl);
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

bool LocalAgent::chooseAttackers(CollectionTN<Creature>& mycreas, StateTN<Creature>& myattackers) {
    return frontEnd.ui.chooseattackers(mycreas, myattackers);
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
