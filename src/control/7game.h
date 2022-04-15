#ifndef OLYMPUS_CLASSES_GAME_7_H
#define OLYMPUS_CLASSES_GAME_7_H

#include "3player.h"
#include "gameplay/game.h"
#include "gameplay/resolvables/stack.h"
#include <memory>
#include <list>

class Agent; class Player;

class Game: public ProtoGame {
    std::list<Player> players;
    bool haswon = false;
public:
    static Game* the_game;

    explicit Game(const std::list<std::unique_ptr<Agent>>& agents);

    void play();

    void stateBasedActions();

    bool hasWon() const { return haswon; }
};

#endif //OLYMPUS_CLASSES_GAME_7_H
