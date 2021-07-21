#include "server.h"
#include "agents/agent.h"
#include "agents/network/networkmanager.h"
//#include "gameplay/7game.h"
#include <thread>

Server* Server::god = nullptr;

Server::Server() {
    if(god != nullptr) exit(1);
    god = this;
}

void Server::launchGame() {
    //if(currentGame != nullptr) return;
    //currentGame = std::make_unique<Game>(players);
    std::cout << "Your game was launched!\n";
}

void Server::addPlayer(playerType type) {
	players.push_front(createAgent(type));
    (*players.begin())->setup();
}

void Server::addPlayerX(const std::list<playerType>& types) {
    std::list<std::thread> setups;
    for(auto desc : types) {
        std::unique_ptr<Agent> agent = createAgent(desc);
        setups.emplace_front(&Agent::setup, &(*agent));
        players.push_front(std::move(agent));
    }
    for(auto& setup : setups) {
        setup.join();
    }
}

Server::~Server() {
    NetworkManager::closeInstances();
}