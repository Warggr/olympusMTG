#include "localagent.h"
#include "gameplay/2cards.h"
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

Target *LocalAgent::chooseTarget(char type) {
    return nullptr; //TODO
}

OptionAction *LocalAgent::chooseOpt(bool sorcerySpeed, Player *pl) {
    return nullptr;
}

void LocalAgent::splitDamage(int power, std::list<std::pair<uint8_t, SpecificTargeter<Creature>>>) {

}

std::list<std::unique_ptr<Card>> LocalAgent::chooseCardsToKeep(std::list<std::unique_ptr<Card>> &list) {
    return std::list<std::unique_ptr<Card>>();
}

bool LocalAgent::keepsHand() {
    return false;
}

bool LocalAgent::chooseAttackers(CollectionTN<Creature> &mycreas, StateTN<Creature> &myattackers) {
    return false;
}

void LocalAgent::chooseBlockers(CollectionTN<Creature> &mycreas, StateTN<Creature> &attackers) {

}
