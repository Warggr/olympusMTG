#include "7game.h"
#include "agents/agent.h"

Game* Game::god;

Game::Game(const std::list<std::unique_ptr<Agent>>& agents) {
    god = this;
    /*for(auto & agent : agents) {
        players.emplace_front(*agent, parseDeck(agent->descriptors));
    }
    Player* oppptr = &players.back();
    for(Player& player : players){
        player.set_opp(oppptr);
        oppptr = &player;
    }
    active_player = &players.front();*/
}
