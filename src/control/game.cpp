#include "3player.h"
#include "7game.h"
#include "agents/agent.h"
#include "oracles/filereader/filereader.h"

Game* Game::god = nullptr;

Game::Game(const std::list<std::unique_ptr<Agent>>& agents) {
    god = this;
    for(auto & agent : agents)
        players.emplace_front(*agent, parseDeck(agent->descriptors));
    for(auto & agent : agents)
        agent->getViewer().connectGame(this);
    Player* oppptr = &players.back();
    for(Player& player : players){
        player.setOpp(oppptr);
        oppptr = &player;
    }
}
