#ifndef OLYMPUS_CLASSES_PLAYER_3_H
#define OLYMPUS_CLASSES_PLAYER_3_H

#include <forward_list>
#include <memory>

#include "1general.h"
#include "../Mana/lib2_mana.h"
#include "../Yggdrasil/headY_yggdrasil.h" //needs to know what a zone is

#define SET_TIME_FLAGS(x) state = (state & 31) | (x);

struct PlayerPreStackElement{
	PreResolvable* preRes;
	std::unique_ptr<Targeter> origin;

	PlayerPreStackElement(PreResolvable* p, Target* org);
};

class CardZone{
private:
    int size;
    char name[10];
    std::forward_list<Card*> cards;
public:
    CardZone(): size(0), name{0} {};
    ~CardZone();

    void init(std::ifstream& textfile, std::ofstream& binaryOut);
    void init(std::ifstream& bFile);
    inline void init_name(const char* n){ for(int i=0; i<10; i++) name[i] = n[i]; };
    void shuffle();
    int drawto(CardZone* target, int nb_cards);
    inline void takeonecard(Card* c){++size; cards.push_front(c); };
    inline void inc_size(int i){ size += i; };
    void describe(char* tmp) const;
    void disp(const Rect& zone) const;
    inline Card* pop_front() {Card* f = cards.front(); cards.pop_front(); return f; };
    inline bool empty(){return cards.empty(); }
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
    iterator<Permanent, false> pbegin();
    iterator<Permanent, false> pend() const;
    iterator<Permanent, true> cpbegin() const;
    iterator<Permanent, true> cpend() const;
};

class Player: public Target, public Damageable{
private: //TODO optimize by taking alignment into account (i.e. put chars after each other)
	std::string name;
	unsigned char state; //MSB t1-t2-t3-land-milled out- 0 life -?-? LSB; t3 = 32
	//upkeep (000) main1(001) afterattack(010) afterblock(011) afterfirstdamage(100) main2(101) end(110) nonactive(111)
	CardZone myzones[3]; //library - graveyard - exile
	std::forward_list<PlayerPreStackElement> prestack;
	char player_id;
	BoardN myboard;
	UIElement* permUI[5];
	UIElement* optZone;
	char nb_mainphase;
public:
#define NBMYOPTS 6
#define LANDOPTS 4
	Option* myoptions[NBMYOPTS+1]{0};
	//Options are sorted in a double-linked list. For convenience, we have pointers to the middle of that list, e.g. to the first land option.
	//castable instant-speed - non-castable instant-speed - castable sorcery - expensive sorcery - land - other - last option
	//Pointers to categories that don't exist are NULL.

	static const std::forward_list<const Phase*> defaultPhaseOrder; //this will be copied and used as a template at the beginning of each turn

	Player* nextopponent;
	Mana highestpool;
	Mana possiblepool;
	Mana manapool;

	Player(const char* deck_name, char id);
	~Player();

	const std::string& get_name() const {static const std::string descr(name); return descr; };
	inline void set_flags(char a){state = state | a;};
	inline void set_opp(Player* opp){nextopponent = opp; };
	inline bool hasplayedland() const {return state & 16; };
	void damage(int nb_damage, Target* origin);

	void disp() const;
	void disp_header(bool highlight = false) const;
	void disp_zone(int nbzone) const;
	Player* iterate_self(DirectioL& direction);
	template <class PermType>
		PermType* iterate_boardsubzone(float offset, DirectioL& direction, PContainer<PermType>* perms, UIElement* ui, bool isactivation);
	Permanent* iterate_boardsubzone(float offset, DirectioL& direction, int xzone, bool istapland);
	Option* first_option(int index) const;
	Option* first_option(int* index) const;
	inline bool has_options(bool sorceryspeed) const;
	void disp_opt(bool sorceryspeed) const;
	inline void addtoPrestack(PreResolvable* triggered_ability, Target* origin){prestack.emplace_front(triggered_ability, origin); };
	bool add_triggers_to_stack();

	bool choose_and_use_opt(bool sorceryspeed);
	void choicephase(bool sorceryspeed);
	
	void insert_permanent(Card* source);
	void remove_permanent(Permanent* perm, int nb_zone);
	template <class PermType> void remove_permanent_inlist(PContainer<PermType>* perms, PermType* asup);
	void resolve_playland(Card* source);
	bool chooseattackers();
	void chooseblockers(StatedCollectionTN<Creature>& attackers, UIElement* attackerDisplay);

	void draw(int nb_cards);
	inline void gain_life(int nb_life) {life += nb_life; };
	void add_mana(char color);
	void puttozone(Card* x, char n);

	void check_too_expensive();
	void empty_pool();

	bool statebasedactions();
	bool turn();

	  void untapstep();
	void upkeepstep();
	void drawstep();
	  void mainphasemainstep();
	void declareattackersstep();
	void declareblockersstep();
	void damagestep();
	  void endstep();
	  void cleanupstep();

	Identifier reload_id() const;
};

#endif //OLYMPUS_CLASSES_GAME_1_H