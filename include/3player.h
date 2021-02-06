#ifndef OLYMPUS_CLASSES_PLAYER_3_H
#define OLYMPUS_CLASSES_PLAYER_3_H

#include <forward_list>
#include <memory>

#include "2cards.h"
//#include "../Yggdrasil/head6_iterators.h"

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

class CardZone{
private:
    char name[10];
    int size;
public:
    CardZone(): name{0}, size(0) {};
    std::forward_list<Card*> cards;

    void init(std::ifstream& textfile, std::ofstream& binaryOut);
    void init(std::ifstream& bFile);
    void init_name(const char* n){ for(int i=0; i<10; i++) name[i] = n[i]; };
    void shuffle();
    int drawto(CardZone* target, int nb_cards);
    void takeonecard(Card* c){++size; cards.push_front(c); };
    void inc_size(int i){ size += i; };
    void describe(char* tmp) const;
    void disp(int x, int y) const;
};

class Player: public Target, public Damageable{
private:
	std::string name;
	unsigned char state; //MSB t1-t2-t3-land-milled out- 0 life -?-? LSB; t3 = 32
	//upkeep (000) main1(001) afterattack(010) afterblock(011) afterfirstdamage(100) main2(101) end(110) nonactive(111)
	Option* myoptions[NBMYOPTS]{0}; //castable: all (mostly instants) - sorceries - lands
	//not castable: too expensive instants - too expensive for the turn - sorcery-speed- castable only next turn (lands, tap)
	CardZone myzones[3]; //library - graveyard - exile
	std::forward_list<PlayerPreStackElement> prestack;
	char player_id;
	BoardN myboard;
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
	Player* iterate_self(DirectioL& direction);
	template <class PermType>
		PermType* iterate_boardsubzone(float offset, DirectioL& direction, PContainer<PermType>* perms, UIElement* ui, bool isactivation);
	Permanent* iterate_boardsubzone(float offset, DirectioL& direction, int xzone, bool istapland);
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
	template <class PermType> void remove_permanent_inlist(PContainer<PermType>* perms, PermType* asup);
	void resolve_playland(Card* source);
	bool chooseattackers();
	void chooseblockers(StatedCollectionTN<Creature>& attackers, UIElement* attackerDisplay);

	void draw(int nb_cards);
	void gain_life(int nb_life) {life += nb_life; };
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

	Identifier reload_id() const;
};

class BoardN: public AbstractN {
public:
    DefaultCollectionTN<Land> mylands;
    DefaultCollectionTN<Artifact> myartos;
    DefaultCollectionTN<Planeswalker> mysuperfriends;
    DefaultCollectionTN<Creature> mycreas;
    StatedCollectionTN<Creature>* myattackers {0};

    BoardN(): mylands(this), myartos(this), mysuperfriends(this), mycreas(this), myattackers(0){};
    ~BoardN();

    void insert(Card* to_add, Player* pl);
    typediterator<Permanent> pbegin();
    typediterator<Permanent> pend() const;
    c_iterator<Permanent> cpbegin() const;
    c_iterator<Permanent> cpend() const;
};

#endif //OLYMPUS_CLASSES_GAME_1_H