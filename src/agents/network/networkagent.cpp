#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fstream>
#include <sstream>
#include <oracles/filereader/visitor.h>
#include "networkagent.h"
#include "networkmanager.h"
#include "gameplay/2cards.h"
#include "oracles/filereader/binarywriter.h"

NetworkAgent::NetworkAgent(): idle(true) {
    NetworkManager::declareAgent(this);
}

void NetworkAgent::specificSetup() {
    NetworkManager::listener_mutex.lock();
    while(idle) { NetworkManager::listen(); }
    NetworkManager::listener_mutex.unlock();
}

const char* NetworkAgent::net_receive() {
    while(!unread) { NetworkManager::listen(); }
    unread = false;
    //std::cout << "Server received " << buffer << "\n";
    return buffer;
}

long NetworkAgent::receive() {
    net_receive();
    return message_length;
}

void NetworkAgent::receiveMessage() {
    //Check if it was for closing , and also read the incoming message
    message_length = read( sockfd , buffer, 1024);
    buffer[message_length] = 0;
    if (message_length == 0) { //Somebody disconnected , get his details and print
        sockaddr_in cli_addr;
        socklen_t clilen = sizeof(cli_addr);
        getpeername(sockfd, (sockaddr*) &cli_addr, &clilen);
        //printf("Host disconnected , ip %s , port %d \n", inet_ntoa(address.sin_addr) , ntohs(address.sin_port));

        //Close the socket
        close( sockfd );
        connected = false;
    }
    else unread = true;
}

void NetworkAgent::setName(const char *read_name) {
    name = std::string(read_name);
    std::cout << "Read name: " << name << "\n";
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

void NetworkAgent::setSock(int sock) {
    idle = false; connected = true;
    sockfd = sock;
}

Target *NetworkAgent::chooseTarget(char type) {
    (void) type; //TODO implement
    return nullptr;
}

void NetworkAgent::splitDamage(int power, std::list<std::pair<uint8_t, SpecificTargeter<Creature>>>& blockers) {
    (void) power; (void) blockers; //TODO implement
}

std::list<uptr<Card>> NetworkAgent::chooseCardsToKeep(std::list<uptr<Card>> &list, uint nbToDiscard) {
    (void) list; (void) nbToDiscard; //TODO implement
    return std::list<std::unique_ptr<Card>>();
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

void NetworkAgent::onDraw(const std::list<uptr<Card>>& cards) {
    char header[2] = { static_cast<char>(CREATE), static_cast<char>(cards.size()) };
    net_send(header, 2);
    for(auto& card: cards) {
        std::stringstream oracle_stream;
        BinaryFileWriter oracle_reader(oracle_stream);
        card->init(oracle_reader);
        net_send(oracle_stream.str());
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
    return receive_file();
}
