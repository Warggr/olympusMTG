#ifndef OLYMPUS_CLASSES_OPTIONS_2_H
#define OLYMPUS_CLASSES_OPTIONS_2_H

class Option{
public:
	Option* prev;
	Option* next;
	bool instantspeed;
	Mana cost;

	Option(Option* pre, Option* nxt, Mana c): prev(pre), next(nxt), instantspeed(0), cost(c){};
	virtual ~Option() {};
	virtual void ragnarok(){ if(next) next->ragnarok(); delete this; }; //called to destroy a full option zone

	void disp_in_list(int* pos, char opttype, Player* pl, bool castable);
	virtual void check_and_pop(int n_of_zone, Player* pl); //default behavior: removes spell from list
	void pop(int, Player*);
	virtual void cast_opt(Player* pl) = 0;
	virtual bool iscastable(Player* pl);
	virtual void disp(int pos, bool highlighted, bool castable) = 0;
};

class SpellOption: public Option{
protected:
	Card* source;
public:
	SpellOption(Card* src, Player* pl, int opttype);
	SpellOption(Card* src, Option* next);
	~SpellOption() {};
	void ragnarok() {delete source; if(next) next->ragnarok(); delete this; };
	
	void disp(int pos, bool highlight, bool castable);
	virtual void cast_opt(Player* pl);
	virtual bool iscastable(Player* pl);
};

class PlayLand: public SpellOption{
public:
	PlayLand(Card* src, Player* pl);
	void cast_opt(Player* pl);
	bool iscastable(Player* pl);
};

//actually PermOption's existence seems more justified to me than Option's existence
//couldn't we just make a Card a Resolvable, or make it an Option only right before being cast?
// it would help if it was a Card in your hand, as you could discard it more easily
//Perhaps Options are useful for exiled cards which you can cast or something?
//Maybe Cards should be a LinkedList and not only their Option wrappers?

class PermOption: public Option {
private:
	Permanent* origin;
	int nbtargets;
	char* typesoftargets; //Spells use the list of targets and the ability list of their Card sources. PermOptions can't do that. 
	Ability* fab;
	bool tapsymbol;
	bool ismanaability;
public:
	PermOption(): Option(0, 0, 0), typesoftargets(0), fab(0), tapsymbol(false), ismanaability(false) {}; //make clear it wasn't initialized
	~PermOption() {};
	void fillout(Mana cost, int nbtars, char* typesoftargets, Ability* fab, bool tapsymbol, bool ismanaability);

	void check_and_pop(int n_of_zone, Player* pl){}; //would check whether option is still applicable
	void cast_opt(Player* pl);
	void straight_cast(Player* pl);

	bool get_tapsymbol(){return tapsymbol; };
	bool get_ismana(){return ismanaability; };
	std::string describe(std::string name);
	void disp(int pos, bool highlight, bool castable){};
};

Option* mergeSortCosts(Option* start, int nb);
Option* merge(Option* start1, Option* start2);

Target* iterate(bool needstarget, Player** pl, char returntypeflags);

#endif //OLYMPUS_CLASSES_OPTIONS_2_H