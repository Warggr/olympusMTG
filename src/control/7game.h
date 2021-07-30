#ifndef OLYMPUS_CLASSES_GAME_7_H
#define OLYMPUS_CLASSES_GAME_7_H

#include <memory>
#include <list>
#include <forward_list>
#include "3player.h"
#include "gameplay/resolvables/5resolvables.h"

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
	void disp() const;
	void addtolog(const char* new_entry);
	void disp_log() const;
	void disp_stack() const;
	void disp_preRes(const Effect_H* preRes, const std::string& origin_name) const;

	void play(){
		while(true){
			for( Player& player : players ) {
			    if(player.turn()) return;
			}
		}
	}

	void stateBasedActions();
};

CardZone parseDeck(std::vector<OracleDescr> deck);

#endif //OLYMPUS_CLASSES_GAME_7_H