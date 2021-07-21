#ifndef OLYMPUS_CLASSES_GAME_7_H
#define OLYMPUS_CLASSES_GAME_7_H

#include <memory>
#include <list>
#include <forward_list>
#include "5resolvables.h"

class Game{
private:
    std::forward_list<std::unique_ptr<Resolvable>> stack;
	std::list<Player> players;
	Player* active_player;
public:
    static Game* god;
	bool haswon = false;

	explicit Game(const std::list<std::unique_ptr<Agent>>& agents);
	~Game();
	void disp() const;
	void addtolog(const char* new_entry);
	void disp_log() const;
	void disp_stack() const;
	void disp_preRes(const Effect_H* preRes, const std::string& origin_name) const;

	void play(){
		while(1){
			if(players[0]->turn() || players[1]->turn()) break;
		}
	}
	void addToStack(std::unique_ptr<Resolvable> rs){ stack.push_front(std::move(rs)); };
	std::unique_ptr<Resolvable> popFromStack();
	bool stackIsEmpty(){return stack.empty(); };
	void removeFromStack(std::unique_ptr<Resolvable>& rs);
	void stateBasedActions();

	Resolvable* iterateStack(float zImposed, DirectioL& direction);
};

#endif //OLYMPUS_CLASSES_GAME_7_H