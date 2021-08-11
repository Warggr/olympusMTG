#ifndef OLYMPUS_CLASSES_GAME_7_H
#define OLYMPUS_CLASSES_GAME_7_H

#include <memory>
#include <list>
#include <forward_list>
#include "3player.h"

class Agent;

class Game{
private:
	std::list<Player> players;
	Player* active_player;
public:
    static Game* god;
	bool haswon = false;

	explicit Game(const std::list<std::unique_ptr<Agent>>& agents);
	//~Game();

	void play(){
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
