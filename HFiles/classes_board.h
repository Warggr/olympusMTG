#ifndef OLYMPUS_CLASSES_BOARD_4_H
#define OLYMPUS_CLASSES_BOARD_4_H

class Permanent: virtual public Target{
protected:
	char name[30];
	char flags; //MSB- - ETBbeforethisTurn - untapped -LSB
	Card* source;
	Player* ctrl;
	char color;
	int nb_actabs;
	PermOption* first_actab;
	Trigger triggers_permanent[3]; //becomes (state) - becomes (special) - is destroyed

public:
	Permanent* prev;
	Permanent* next;

	Permanent(Card* source, Player* pl, int nb_zone);
	void ragnarok(){delete source; if(next) next->ragnarok(); delete this; };
	virtual ~Permanent(){};
	virtual void describe(char* tmp);
	virtual void disp(int x, int y, bool highlight);

	virtual char target_type(){return 0x40; };

	char get_flags(){return flags; };
	const char* get_name(){return name; };

	void set_flags(char flg){flags = flags | flg; };
	void reset_flags(char flg){flags = flags & ~flg; };
	void set_board_flags(char flg){set_flags(flg); if(next) next->set_board_flags(flg); };
	virtual void pop();

	virtual void untap(bool forall){flags = flags | 1; if(forall && next) next->untap(true); };
	virtual void activate();
	bool directactivate();
	void destroy(bool forall);
};

class Artifact: public Permanent{
public:
	Artifact(Card* src, Player* pl): Permanent(src, pl, 1){};
	Artifact* heartofsteel(){return this; };
};

class Creature: public Permanent, public Damageable{
private:
	Trigger triggers_creature[4]; //attacks - blocks - becomes blocked - isn't blocked
	int power;
	int toughness;
	int nb_counters;
public:
	int nb_blockers;
	Creature* assigned_bl;

	Creature(Card* src, Player* pl);
	void describe(char* tmp);
	void disp(int x, int y, bool highlight);
	void hit(Damageable* tgt);
	int get_power(){return power; };
	int get_toughness(){return toughness; };
	void pop();
	char target_type(){return 0x50; }; //Permanent (0x40) and Creature (0x10)
};

class Planeswalker: public Permanent, public Damageable{
public:
	Planeswalker(Card* src, Player* pl): Permanent(src, pl, 2){};
	char target_type(){return 0x48; }; //Permanent and Planeswalker
};

class Land: public Permanent{
public:
	Land(Card* src, Player* pl);

	void untap(bool forall);
};

#endif //OLYMPUS_CLASSES_BOARD_4_H