#include "3player.h"
#include "7game.h"
#include "agents/agent.h"
#include "oracles/filereader/readfile.h"

Game* Game::the_game = nullptr;

Game::Game(const std::list<std::unique_ptr<Agent>>& agents) {
    the_game = this;
    for(auto& agent : agents){
        auto deck = parseDeck(agent->descriptors);
        agent->getViewer().connectDeck(deck);
        players.emplace_front(*agent, std::move(deck));
    }
    for(auto& agent : agents)
        agent->getViewer().connectGame(this);
    Player* oppptr = &players.back();
    for(Player& player : players){
        player.setOpp(oppptr);
        oppptr = &player;

        gamers.push_back(&player);
    }
}
