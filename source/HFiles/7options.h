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

	virtual void check_and_pop(int n_of_zone, Player* pl); //default behavior: removes spell from list
	void pop(int, Player*);
	virtual Resolvable* cast_opt(Player* pl) = 0;
	virtual bool iscastable(const Player* pl) const;
	virtual void disp(int y, int z, bool highlight, bool castable) const = 0;
};

class SpellOption: public Option{
protected:
	Card* source;
public:
	SpellOption(Card* src, Player* pl, int opttype);
	SpellOption(Card* src, Option* next);
	~SpellOption() {};
	void ragnarok() {delete source; if(next) next->ragnarok(); delete this; };
	
	void disp(int y, int z, bool highlight, bool castable) const;
	virtual Resolvable* cast_opt(Player* pl);
	virtual bool iscastable(const Player* pl) const;
};

class PlayLand: public SpellOption{
public:
	PlayLand(Card* src, Player* pl);
	Resolvable* cast_opt(Player* pl);
	bool iscastable(const Player* pl) const;
};

//actually PermOption's existence seems more justified to me than Option's existence
//couldn't we just make a Card a Resolvable, or make it an Option only right before being cast?
// it would help if it was a Card in your hand, as you could discard it more easily
//Perhaps Options are useful for exiled cards which you can cast or something?
//Maybe Cards should be a LinkedList and not only their Option wrappers?

class PermOption: public Option {
private:
	Permanent* origin;
	PreResolvable* effects; //Spells use the PreRes, (aka the list of targets and the ability list) of their Card sources. PermOptions can't do that. 
	bool tapsymbol;
	bool ismanaability;
public:
	PermOption(): Option(0, 0, 0), effects(0), tapsymbol(false), ismanaability(false) {}; //make clear it wasn't initialized
	~PermOption() {delete effects; };
	void fillout(Mana cost, PreResolvable* preRes, bool tapsymbol, bool ismanaability);
	void read_binary(std::ifstream& bFile);
	void write_binary(std::ofstream& bFile) const;

	void check_and_pop(int n_of_zone, Player* pl){}; //would check whether option is still applicable
	Resolvable* cast_opt(Player* pl);
	void straight_cast(Player* pl);

	bool get_tapsymbol() const {return tapsymbol; };
	bool get_ismana() const {return ismanaability; };
	std::string describe(std::string name) const;
	void disp(int y, int z, bool highlight, bool castable) const {};
};

Option* mergeSortCosts(Option* start, int nb);
Option* merge(Option* start1, Option* start2);

#endif //OLYMPUS_CLASSES_OPTIONS_2_H