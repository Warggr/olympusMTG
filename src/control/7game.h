#ifndef OLYMPUS_CLASSES_GAME_7_H
#define OLYMPUS_CLASSES_GAME_7_H

#include <memory>
#include <list>
#include <forward_list>
#include "3player.h"
#include "gameplay/resolvables/stack.h"

class Agent; class Player;

class Game{
public:
    Stack stack;
    std::list<Player> players;
    static Game* god;
    bool haswon = false;

    explicit Game(const std::list<std::unique_ptr<Agent>>& agents);

    void play();

    void stateBasedActions();
};

#endif //OLYMPUS_CLASSES_GAME_7_H
