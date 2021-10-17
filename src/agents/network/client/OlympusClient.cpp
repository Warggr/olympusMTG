#include "OlympusClient.h"
#include <thread>
#include <iostream>
#include <unistd.h>

void OlympusClient::play() {
    /*const char* request = network.net_receive();
    switch(request[0]) {
        case 'X': frontEnd.create(request + 1); break;
        case 'U': frontEnd.update(request + 1); break;
        case 'D': frontEnd.del(request + 1); break;
        case 'B': frontEnd.bulkOp(request + 1); break;
        case '?': query(request + 1); break;
    }*/
}

void OlympusClient::query(const char *question) {
    (void) question; //TODO
}

void OlympusClient::start() {
    /*do {
        const char* hand = network.net_receive();
        frontEnd.create(hand);
    } while(frontEnd.askMulligan());
    try {
        while (true) play();
    } catch(const UIClosedException& x) {

    }*/
    while(true);
    //sleep(10);
}

void OlympusClient::init(const char *hostIp, const char *playerName) {
    std::thread net_thread( &NetworkClient::init, &network, hostIp, playerName); //while we're waiting for the server to answer...

    auto filename = agent.getDeckFile(); //...we ask the player for his deck
    net_thread.join();
    std::cout << "Net thread joined\n";
    network.send_file(*filename); //once both are done, we send the file
}

void OlympusClient::init() {
    init(agent.getLogin().c_str(), agent.getName().c_str());
}

void OlympusClient::init(const char *hostIp, const char *playerName, const char* fileName) {
    std::thread net_thread( &NetworkClient::init, &network, hostIp, playerName);
    net_thread.join(); //why doesn't it work without threads?
    std::ifstream deck ( fileName, std::ios::in );
    network.send_file(deck);
}
