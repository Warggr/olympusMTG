#include <sys/socket.h>
#include <netinet/in.h>
#include <sstream>
#include <oracles/filereader/visitor.h>
#include "networkagent.h"
#include "gameplay/2cards.h"
#include "oracles/filereader/binary/binarywriter.h"

NetworkAgent::NetworkAgent() = default;

void NetworkAgent::specificSetup() {
    networker.waitForConnection();
}

unsigned int parse_number(const char* line, unsigned int& pos) {
    unsigned int ret = 0;
    while (true) {
        if (line[pos] == ' ') return ret;
        else if ('0' <= line[pos] && line[pos] <= '9')
            ret = 10 * ret + line[pos] - '0';
        else throw DeckbuildingError("Not a number");
        pos++;
    }
}

Target *NetworkAgent::chooseTarget(char type) {
    (void) type; //TODO implement
    return nullptr;
}

void NetworkAgent::splitDamage(int power, std::list<std::pair<uint8_t, SpecificTargeter<Creature>>>& blockers) {
    (void) power; (void) blockers; //TODO implement
}

std::list<CardWrapper> NetworkAgent::chooseCardsToKeep(std::list<CardWrapper> &list, uint nbToDiscard) {
    (void) list; (void) nbToDiscard; //TODO implement
    return std::list<CardWrapper>();
}

bool NetworkAgent::keepsHand(const fwdlist<uptr<Card>>& cards) {
    (void) cards;
    return false; //TODO implement
}

Option * NetworkAgent::chooseOpt(bool sorcerySpeed, Player *pl) {
    (void) sorcerySpeed; (void) pl; //TODO implement
    return nullptr;
}

bool NetworkAgent::chooseAttackers(Y_Hashtable<Creature>& mycreas) {
    (void) mycreas; //TODO implement
    return false;
}

void NetworkAgent::chooseBlockers(Y_Hashtable<Creature>& mycreas, StateTN<Creature>& attackers) {
    (void) mycreas; (void) attackers; //TODO implement
}

void NetworkAgent::connectGame(Game* gm) {
    game = gm;
}

void NetworkAgent::onDraw(const std::list<CardWrapper>& cards) {
    char header[2] = { static_cast<char>(CREATE), static_cast<char>(cards.size()) };
    networker.send(header, 2);
    for(auto& card: cards) {
        std::stringstream oracle_stream;
        BinaryFileWriter oracle_reader(oracle_stream);
        const_cast<CardWrapper&>(card)->init(oracle_reader);
        networker.send(oracle_stream.str());
    }
}

uint NetworkAgent::chooseAmong(std::vector<CardOption*> opts) {
    (void) opts; return 0; //TODO
}

uint NetworkAgent::chooseAmong(std::vector<PermOption*> opts) {
    (void) opts; return 0; //TODO
}

std::string NetworkAgent::getLogin() {
    return "127.0.0.1"; //value irrelevant - you're not supposed to chain two network agents
}

uptr<std::istream> NetworkAgent::getDeckFile() {
    return networker.receive_file();
}

void NetworkAgent::registerMe(Player*) {}
