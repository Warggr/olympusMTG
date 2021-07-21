#include "7game.h"
#include "agents/agent.h"

Game* Game::god;

Game::Game(const std::list<std::unique_ptr<Agent>>& agents) {
    god = this;
    for(auto & agent : agents) {
        players.emplace_front(agent, parse_deck(agent->getDeck()));
    }
}

std::unique_ptr<Resolvable> Game::popFromStack(){
    if(stack.empty()) return nullptr;
    auto ret = std::move(stack.front());
    stack.pop_front();
    return ret;
}

void Game::removeFromStack(std::unique_ptr<Resolvable>& rs){
    stack.remove(rs);
}