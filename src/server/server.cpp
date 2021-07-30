#include "networkagent.h"
#include "agents/agent.h"
#include "agents/network/networkmanager.h"
#include "control/7game.h"
#include <thread>

Server* Server::god = nullptr;

Server::Server() {
    if(god != nullptr) exit(1);
    god = this;
}

void Server::launchGame() {
    if(currentGame != nullptr) return;
    currentGame = std::make_unique<Game>(players);
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

namespace target_type {
    const flag_t tars1[] = { damageable, nonnegative };
    const flag_t tars2[] = { player, nonnegative };
    const flag_t tars3[] = { player, number };
    const flag_t tars4[] = { player, added_mana };
    const flag_t tars5[] = { permanent };
    const flag_t tars6[] = { resolvable };
    const flag_t tars7[] = { permanent, strictpositive };
    const flag_t tars8[] = { permanent };
    const flag_t* target_types[] = { tars1, tars2, tars3, tars3, tars4, tars5, tars5, tars6, tars7, tars8 };
    const int target_numbers[] = { 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1 };
}
