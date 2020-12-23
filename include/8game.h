#ifndef OLYMPUS_CLASSES_GAME_8_H
#define OLYMPUS_CLASSES_GAME_8_H
#include "3player.h"

class Game{
private:
	std::forward_list<Resolvable*> stack; //Guideline: the stack has full ownership of Resolvables. Any function calling popfromstack is responsible for deleting the resolvable
	Player* players[2];
	Player* active_player;
	char* logbook[LOGLEN];
	UIElement* stack_ui;
	UIElement* logbook_ui;
public:
	bool haswon;

	Game(const char* deck_1, const char* deck_2){Game(deck_1, deck_2, 0); };
	Game(const char* deck_1, const char* deck_2, char debug_flags);
	~Game();
	void disp() const;
	void addtolog(const char* new_entry);
	void disp_log() const;
	void disp_stack() const;
	void disp_preRes(const PreResolvable* preRes, const std::string& origin_name) const;

	void play(){
		while(1){
			if(players[0]->turn() || players[1]->turn()) break;
		}
	}
	void addtostack(Resolvable* rs){stack.push_front(rs); };
	Resolvable* popfromstack();
	bool stackisempty(){return stack.empty(); };
	void remove_from_stack(Resolvable* rs);
	void statebasedactions();

	Resolvable* iterate_stack(float zImposed, char* direction);
};

#endif //OLYMPUS_CLASSES_GAME_13_H