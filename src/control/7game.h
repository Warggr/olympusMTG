#ifndef OLYMPUS_CLASSES_GAME_7_H
#define OLYMPUS_CLASSES_GAME_7_H

#include <memory>
#include <list>
#include <forward_list>
#include "3player.h"
#include "gameplay/resolvables/stack.h"

class Agent;

class Game{
private:
	Player* active_player;
public:
    std::list<Player> players;
    static Game* god;
	bool haswon = false;

	explicit Game(const std::list<std::unique_ptr<Agent>>& agents);
	//~Game();

	void play(){
	    Stack stack;
		while(true){
			for( Player& player : players ) {
			    if(player.turn()) return;
			}
		}
	}

	void stateBasedActions();
};

CardZone parseDeck(const std::vector<OracleDescr>& deck);

#endif //OLYMPUS_CLASSES_GAME_7_H
