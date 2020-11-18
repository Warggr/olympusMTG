#ifndef OLYMPUS_CLASSES_GAME_1_H
#define OLYMPUS_CLASSES_GAME_1_H

#define NBMYOPTS 5
#define INSTANTOPTS 0
#define SORCERYOPTS 1
#define LANDOPTS 2
#define TOOEXPENSIVE 3
#define NEXTTURNONLY 4

//possible options will be sorted by (&INSTANT) instants - (&LANDS) lands - (&SORCERY) sorceries

class Player: public Damageable{
private:
	char name[10];
	unsigned char state; //MSB t1-t2-t3-land-milled out- 0 life -?-? LSB; t3 = 32
	//upkeep (000) main1(001) afterattack(010) afterblock(011) afterfirstdamage(100) main2(101) end(110) nonactive(111)
	Option* myoptions[NBMYOPTS]; //not castable: too expensive instants - too expensive for the turn - sorcery-speed- castable only next turn (lands, tap) - 
	//castable: all (mostly instants) - sorceries or last instant - land or last sorcery
	CardZone myzones[3]; //library - graveyard - exile
	//Resolvable* prestack;
public:
	int shown_depth;
	char shown_direction;
	Player* nextopponent;
	Game* metagame;
	Permanent* fperm[5]; //land - arti - plaw - crea - attack
	Mana highestpool;
	Mana possiblepool;
	Mana manapool;

	Player(FILE* myfile, Game* gm, char isup);
	~Player();

	void set_flags(char a){state = state | a;};
	void set_myoptions(int n, Option* op) {myoptions[n] = op; };
	void set_opp(Player* opp){nextopponent = opp; };
	Option* get_myoptions(int n) {return myoptions[n]; };
	bool hasplayedland(){return state & 16; };

	void disp();
	void disp_header();
	void disp_zone(int nbzone);
	Player* iterate_self(char* direction);
	Permanent* iterate_boardsubzone(char* direction, int xzone, bool istapland);
	bool disp_opt(bool sorceryspeed);
	int disp_myopts(char* pos, int typeofopt, char a);
	void dispOptsOfCertainType(int* pos, int type, bool castable);
	void clear_opts();
	char target_type(){return 0x80; };

	Option* choose_opt(bool sorceryspeed);
	bool choose_and_use_opt(bool sorceryspeed);
	void choicephase(bool sorceryspeed);
	void get_up(Option** iter, int* pos, int* metapos);
	void get_down(Option** iter, int* pos, int* metapos);
	
	void chooseattackers();
	void dispattackers();
	void dispblockers(int, Creature*, int);

	void draw(int nb_cards);
	void gain_life(int nb_life) {life += nb_life; };
	void set_life(int nb_life) {life = nb_life; };
	void add_mana(char color);
	void puttozone(Card* x, char n);
	void check_too_expensive();
	void landisplayed(unsigned* producedmana);

	void end_phase();
	void begin_phase();
	void empty_pool();
	void untapstep();
	void upkeepstep();
	void drawstep();
	void mainphase();
	void declareattackersstep();
	void declareblockersstep();
	void chooseblockers();
	void damagestep();
	void takebackfighters(Permanent* last_fighter);
	void endstep();

	bool turn();
};

#define DBG_IMPORTANT 0x1
#define DBG_READFILE 0x2
#define DBG_X_READFILE 0x4
#define DBG_INTERNAL 0x8
#define DBG_TARGETING 0x10
#define DBG_RAGNAROK 0x20

class Game{
private:
	Resolvable* stack;
	Player* players[2];
	Player* active_player;
	char* logbook[LOGLEN];
	char wanted_debug;
public:
	bool haswon;

	Game(const char* filename){Game(filename, 0); };
	Game(const char* filename, char debug_flags);
	~Game();
	void initgraph();
	void deletegraph();
	void disp();
	void addtolog(const char* new_entry);
	void message(const char* entry);
	void disp_log();
	void disp_stack();

	void play(){
		while(1){
			if(players[0]->turn() || players[1]->turn()) break;
		}
	}
	void addtostack(Resolvable* rs){rs->next = stack; stack = rs; };
	Resolvable* popfromstack(){Resolvable* tmp=stack; if(stack) stack = tmp->next; return tmp; };
	bool stackisempty(){return !stack; };
	void remove_from_stack(Resolvable* rs);
	void statebasedactions();

	Resolvable* iterate_stack(char* direction);
};

#endif //OLYMPUS_CLASSES_GAME_1_H