#ifndef OLYMPUS_CLASSES_BOARD_4_H
#define OLYMPUS_CLASSES_BOARD_4_H

class Permanent: virtual public Target{
protected:
	std::string name;
	char flags; //MSB- - ETBbeforethisTurn - untapped -LSB
	char keywords; //indestructible -
	Card* source;
	Player* ctrl;
	char color;
	int nb_actabs;
	PermOption* first_actab;
	TriggerEvent triggers_permanent[4]; //becomes (state) - becomes (special) - ETB - LTB

public:
	Permanent(Card* source, Player* pl, int nb_zone);
	virtual ~Permanent(){};
	virtual std::string describe() const;
	virtual void disp(int left, int top, int width, int height, bool highlight = false) const;

	char get_flags() const {return flags; };
	const std::string& get_name() const {return name; };

	void set_flags(char flg){flags = flags | flg; };
	void reset_flags(char flg){flags = flags & ~flg; };
	virtual void ragnarok();

	virtual void untap(){flags = flags | 1; };
	virtual void activate();
	bool directactivate();
	virtual void destroy();
	virtual void exile();
};

class Artifact: public Permanent{
public:
	Artifact(Card* src, Player* pl): Permanent(src, pl, 1){};
};

class Creature: public Permanent, public Damageable{
private:
	TriggerEvent triggers_creature[4]; //attacks - blocks - becomes blocked - isn't blocked
	int power;
	int toughness;
	int nb_counters;

	bool is_attacking;
	bool is_block; //used for both blocking and blocked
	std::list<Creature*> assigned_bl; //this might actually become invalidated if the attacker dies midcombat. A Targeter would be better, but this is for another day
public:
	Creature(Card* src, Player* pl);
	std::string describe() const;

	void disp(int left, int top, int width, int height, bool highlight) const;

	void damage(int nb_damage, Target* origin = 0){ life -= nb_damage; is_damaged.trigger(ctrl, new Targeter(this)); };
	void hit(Damageable* tgt);
	void add_blocker(Creature* bl);
	void set_blocking(){is_block = true; };
	void resolve_attack(Player* nextopponent);
	int get_power() const {return power; };
	int get_toughness() const {return toughness; };
	void destroy(bool wasattacking);
	void exile(bool wasattacking);
};

class Planeswalker: public Permanent, public Damageable{
public:
	Planeswalker(Card* src, Player* pl): Permanent(src, pl, 2), Damageable((int) src->get_flavor_text()[0], src){};
	char target_type() const{return 0x48; }; //Permanent and Planeswalker
	void damage(int nb_damage, Target* origin){ life -= nb_damage; is_damaged.trigger(ctrl, new Targeter(this)); };
};

class Land: public Permanent{
public:
	Land(Card* src, Player* pl);

	void disp(int left, int top, int width, int height, bool highlight) const;
	void untap();
};

#endif //OLYMPUS_CLASSES_BOARD_4_H