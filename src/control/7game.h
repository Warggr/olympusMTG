#ifndef OLYMPUS_CLASSES_GAME_7_H
#define OLYMPUS_CLASSES_GAME_7_H

#include <memory>
#include <list>
#include "3player.h"
#include "gameplay/resolvables/stack.h"

class Agent; class Player;

class Game {
    Stack stack;
    std::list<const Gamer*> gamers;
    std::list<Player> players;
    bool haswon = false;
public:
    static Game* god;

    explicit Game(const std::list<std::unique_ptr<Agent>>& agents);

    void play();

    void stateBasedActions();

    bool hasWon() const { return haswon; }
    const Stack& getStack() const { return stack; }
    const std::list<const Gamer*>& getGamers() const { return gamers; }
    const std::list<Player>& getPlayers() const { return players; } 
};

#endif //OLYMPUS_CLASSES_GAME_7_H
