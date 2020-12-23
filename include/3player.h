#ifndef OLYMPUS_CLASSES_PLAYER_3_H
#define OLYMPUS_CLASSES_PLAYER_3_H

#include <forward_list>
#include <memory>
#include "../Mana/head2_mana.h"
#include "1general.h"
#include "2cards.h"

#define NBMYOPTS 5
#define INSTANTOPTS 0
#define SORCERYOPTS 1
#define LANDOPTS 2
#define TOOEXPENSIVE 3
#define NEXTTURNONLY 4

#define SET_TIME_FLAGS(x) state = (state & 31) | (x);

//possible options will be sorted by (&INSTANT) instants - (&LANDS) lands - (&SORCERY) sorceries

struct PlayerPreStackElement{
	PreResolvable* preRes;
	std::unique_ptr<Targeter> origin;

	PlayerPreStackElement(PreResolvable* p, Target* org);
};

class Player: public Damageable{
private:
	char name[10];
	unsigned char state; //MSB t1-t2-t3-land-milled out- 0 life -?-? LSB; t3 = 32
	//upkeep (000) main1(001) afterattack(010) afterblock(011) afterfirstdamage(100) main2(101) end(110) nonactive(111)
	Option* myoptions[NBMYOPTS]; //castable: all (mostly instants) - sorceries - lands
	//not castable: too expensive instants - too expensive for the turn - sorcery-speed- castable only next turn (lands, tap)
	CardZone myzones[3]; //library - graveyard - exile
	std::forward_list<PlayerPreStackElement> prestack;
	char player_id;
	std::list<Land> mylands;
	std::list<Creature> mycreas;
	std::list<Creature> myattackers;
	std::list<Artifact> myartos;
	std::list<Planeswalker> mysuperfriends;
	UIElement* permUI[5];
	UIElement* optZone;
	char nb_mainphase;
public:
	static const std::forward_list<const Phase*> defaultPhaseOrder; //this will be copied and used as a template at the beginning of each turn

	Player* nextopponent;
	Mana highestpool;
	Mana possiblepool;
	Mana manapool;

	Player(const char* deck_name, char id);
	~Player();

	const std::string& get_name() const {static const std::string descr(name); return descr; };
	void set_flags(char a){state = state | a;};
	void set_myoptions(int n, Option* op) {myoptions[n] = op; };
	void set_opp(Player* opp){nextopponent = opp; };
	Option* get_myoptions(int n) const {return myoptions[n]; };
	bool hasplayedland() const {return state & 16; };
	void damage(int nb_damage, Target* origin);

	void disp() const;
	void disp_header(bool highlight = false) const;
	void disp_zone(int nbzone) const;
	Player* iterate_self(char* direction);
	template <class PermType>
		PermType* iterate_boardsubzone(float offset, char* direction, std::list<PermType>& perms, UIElement* ui, bool isactivation);
	Permanent* iterate_boardsubzone(float offset, char* direction, int xzone, bool istapland);
	bool disp_opt(bool sorceryspeed) const;
	void dispOptsOfCertainType(int y, int z, int dy, int dz, int* pos, int type, bool castable) const;
	void addtoPrestack(PreResolvable* triggered_ability, Target* origin){prestack.emplace_front(triggered_ability, origin); };
	bool add_triggers_to_stack();

	bool choose_and_use_opt(bool sorceryspeed);
	void choicephase(bool sorceryspeed);
	bool get_up(Option*& iter, int& pos, int& metapos) const;
	bool get_down(Option*& iter, int& pos, int& metapos) const;
	
	void insert_permanent(Card* source);
	void remove_permanent(Permanent* perm, int nb_zone);
	template <class PermType> void remove_permanent_inlist(std::list<PermType>& perms, PermType* asup);
	void resolve_playland(Card* source);
	bool chooseattackers();
	void chooseblockers(std::list<Creature>& attackers, UIElement* attackerDisplay);
	void for_each_perm(void (*fun)(Permanent& p));

	void draw(int nb_cards);
	void gain_life(int nb_life) {life += nb_life; };
	void set_life(int nb_life) {life = nb_life; };
	void add_mana(char color);
	void puttozone(Card* x, char n);
	void check_too_expensive();

	void empty_pool();

	void untapstep();
	void upkeepstep();
	void drawstep();
	  void mainphasemainstep();
	void declareattackersstep();
	void declareblockersstep();
	void damagestep();
	  void endstep();
	  void cleanupstep();

	bool statebasedactions();
	bool turn();
};

/*void Game::remove_from_stack(Resolvable* rs){
	if(stack.front() == rs) stack.pop_front();
	else{
		Resolvable* iter;
		for(iter = stack; iter->next != rs; iter = iter->next)
			if(iter == 0)
				god.gdebug(DBG_TARGETING | DBG_IMPORTANT) << "Error: countered Resolvable not in stack\n";
		iter->next = rs->next;
	}
}*/

#endif //OLYMPUS_CLASSES_GAME_1_H