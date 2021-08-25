#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fstream>
#include <oracles/filereader/visitor.h>
#include "networkagent.h"
#include "networkmanager.h"
#include "gameplay/2cards.h"

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

unsigned int parse_number(const char *line, unsigned int& pos) {
    unsigned int ret = 0;
    while (true) {
        if (line[pos] == ' ') return ret;
        else if ('0' <= line[pos] && line[pos] <= '9')
            ret = 10 * ret + line[pos] - '0';
        else throw DeckbuildingError("Not a number");
        pos++;
    }
}

std::vector<OracleDescr> NetworkAgent::getDeck() {
    std::vector<OracleDescr> ret;
    while(true) { //read all segments
        net_receive();
        //for(int i=0; i<message_length + 3; i++) std::cout << "[" << i << "]: " << (buffer[i] == 0 ? '#' : buffer[i]) << "\n";
        unsigned int start = 0;
        while(true) { //read all cards
            //std::cout << "[" << start << "]: " << (buffer[start] == 0 ? '#' : buffer[start]) << "\n";
            if(start == message_length) break;
            if(start > message_length) throw NetworkError();
            if(buffer[start] == '\n') return ret; //\n is the ending character, because \0 is used as line delimiter
            auto nb_cards = parse_number(buffer, start);
            start++; //start points to the position after the space
            ret.emplace_back(customcard, nb_cards, buffer + start);
            start += ret.back().initializer.size() + 1;
        }
    }
}

void NetworkAgent::setSock(int sock) {
    idle = false; connected = true;
    sockfd = sock;
}

Target *NetworkAgent::chooseTarget(char type) {
    return nullptr; //TODO
}

void NetworkAgent::splitDamage(int power, std::list<std::pair<uint8_t, SpecificTargeter<Creature>>>& blockers) {

}

std::list<std::unique_ptr<Card>> NetworkAgent::chooseCardsToKeep(std::list<std::unique_ptr<Card>> &list) {
    return std::list<std::unique_ptr<Card>>();
}

bool NetworkAgent::keepsHand() {
    return false;
}

uptr<OptionAction> NetworkAgent::chooseOpt(bool sorcerySpeed, Player *pl) {
    return nullptr;
}

bool NetworkAgent::chooseAttackers(CollectionTN<Creature> &mycreas, StateTN<Creature> &myattackers) {
    return false;
}

void NetworkAgent::chooseBlockers(CollectionTN<Creature> &mycreas, StateTN<Creature> &attackers) {

}
