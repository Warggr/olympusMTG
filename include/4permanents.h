#ifndef OLYMPUS_CLASSES_PERMANENTS_4_H
#define OLYMPUS_CLASSES_PERMANENTS_4_H

#include "1general.h"
#include "../HFiles/headI_identifiers.h"

class Permanent: public Target{
protected:
	TriggerEvent triggers_permanent[4]; //becomes (state) - becomes (special) - ETB - LTB
	Card* source;
	Player* ctrl;
	PermOption* first_actab;
	ModifListNode* existing_statics;
	int nb_actabs;
	char flags; //MSB- - ETBbeforethisTurn - untapped -LSB
	char keywords; //indestructible -
	char color;
public:
	Permanent(Card* source, Player* pl, int nb_zone);
	virtual ~Permanent() = default;
	void ragnarok_perm();
	virtual std::string describe() const;
	virtual void disp(int left, int top, int width, int height, bool highlight = false) const;

	char get_flags() const {return flags; };
	void reset_flags(char flg){flags = flags & ~flg; };

	virtual void untap(){flags = flags | 1; };
	virtual void declare_beginturn(){untap(); flags = flags | 2; };
	virtual void activate();
	bool directactivate();
	virtual void destroy();
	virtual void exile();
	//void merge_source(Card* src_copy){delete source; source = src_copy; };
};

class Artifact: public Permanent{
public:
	static constexpr Identifier def_identifier = construct_id_perm(artifact, 0, 0);

	Artifact(Card* src, Player* pl): Permanent(src, pl, 1){};
	Identifier reload_id() const override;
};

class Creature: public Permanent, public Damageable{
private:
	TriggerEvent triggers_creature[4]; //attacks - blocks - becomes blocked - isn't blocked
	int self_power, self_toughness; //layer 7a, characteristics-defining effects
	int set_power, set_toughness; //layer 7b
	int added_power{0}, added_toughness{0}; //layer 7c. By default these values are volatile and are reset at end of turn
	int nb_counters{0};
	bool pt_switched{false}; //layer 7d

	bool is_attacking{false}, is_block{false}; //used for both blocking and blocked
	std::list<Creature*> assigned_bl; //this might actually become invalidated if the attacker dies midcombat. A Targeter would be better, but this is for another day

public:
	static constexpr Identifier def_identifier = construct_id_perm(creature, 0, 0);

	Creature(Card* src, Player* pl);
	std::string describe() const;

	void disp(int left, int top, int width, int height, bool highlight) const;

	void damage(int nb_damage, Target* origin = 0);
	void hit(Damageable* tgt) const;
	void add_blocker(Creature* bl);
	void set_blocking(){is_block = true; };
	void resolve_attack(Player* nextopponent);
	int get_power() const {if(pt_switched) return set_power + added_power; else return set_toughness + added_toughness; };
	int get_toughness() const {if(pt_switched) return set_toughness + added_toughness; else return set_power + added_power; };
	void plus_power(char dpower){added_power += dpower; };
	void plus_toughness(char dtoughness){added_toughness += dtoughness; };

    Identifier reload_id() const override;
};

class Planeswalker: public Permanent, public Damageable{
    PreResolvable* loyalty_abs;
    char* loyalty_costs;
    int loyalty;
    bool thisturn;
public:
	static constexpr Identifier def_identifier = construct_id_perm(planeswalker, 0, 0);

	Planeswalker(Card* src, Player* pl);
	char target_type() const{return 0x48; }; //Permanent and Planeswalker
	void damage(int nb_damage, Target* origin);
	void activate();
	void declare_beginturn(){Permanent::declare_beginturn(); thisturn = false; };

	Identifier reload_id() const override;
};

class Land: public Permanent{
public:
	static constexpr Identifier def_identifier = construct_id_perm(land, 0, 0);

	Land(Card* src, Player* pl);

	void disp(int left, int top, int width, int height, bool highlight) const;
	void untap();

	Identifier reload_id() const override;
};

#endif //OLYMPUS_CLASSES_PERMANENTS_4_H