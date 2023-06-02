#include "server.h"
#include "agents/agent.h"
#include "network/server/networkmanager.h"
#include "control/3player.h" //some kind of implicit deletion of Players when creating a list
#include "control/7game.h"
#include <thread>

Server::Server(){
}

void Server::launchGame() {
    assert(currentGame == nullptr);
    currentGame = std::make_unique<Game>(players);
    std::cout << "Your game was launched!\n";
    currentGame->play();
}

void Server::addPlayer(PlayerType type) {
    AgentContext context = {
        ENABLE_IF_NETWORK(manager)
    };
	players.push_front(agent_factory(type, context));
    (*players.begin())->setup();
}

void Server::addPlayers(const std::list<PlayerType>& types) {
    std::list<std::thread> setups;
    AgentContext context = {
        ENABLE_IF_NETWORK(manager)
    };
    for(auto desc : types) {
        std::unique_ptr<Agent> agent = agent_factory(desc, context);
        setups.emplace_front(&Agent::setup, agent.get());
        players.push_front(std::move(agent));
    }
    for(auto& setup : setups) {
        setup.join();
    }
}

Server::~Server() {
}

void Server::addToLog(std::string_view message) {
    for(const auto& agent : getAgents()) {
        agent->getViewer().message(message);
    }
}
