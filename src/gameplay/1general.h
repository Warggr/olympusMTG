#ifndef OLYMPUS_GENERALS_1_H
#define OLYMPUS_GENERALS_1_H

#include "headE_enums.h"
#include <forward_list>
#include <string>
#include <list>

using Identifier = int;

class Player; class Target; class Trigger_H; class Targeter; class Card;

class TriggerEvent{ //contains all X-action triggers of a certain object, i.e. all Death triggers of one permanent.
	//a Trigger contains 3 constraints about the actions that might trigger him: whenever <a Beeble> deals <combat damage> to <a Brushwagg>, ...
	std::forward_list<Trigger_H*> all_triggers;
	int id_args[3];
public:
	void trigger(Player* pl, Target* origin) const;
	void add_trigger(Trigger_H* trig){ all_triggers.push_front(trig); };
	std::string describe(int typeoftrig, std::string cardname) const;
};

class GameObject{ //Players, Permanents, Resolvables, Cards, and maybe enums or zones or abilities (as in: abilities can't be activated) later on? (Or, would that restriction be put on the origin, or the resolvable?)
private:
    Identifier identifier;
    bool id_still_correct;
public:
    GameObject(): identifier(0), id_still_correct(false) {};
    void invalidate(){ id_still_correct = false; };
    virtual Identifier reload_id() const = 0;
    Identifier identify();
};

class Target: public GameObject{ //Permanents, Players, Cards (not Oracles!), and Resolvables
protected:
	std::list<Targeter*> to_target;
	flag_t t_type{0};
	char target_flags; //hexproof
	const std::string& name;
public:
	Target(const std::string& name): target_flags(0), name(name) {};
	//Target(SameObjectTN* arch, char type, std::string name): GameObject(arch), t_type(type), target_flags(0), name(std::move(name)) {};
	virtual ~Target();
	void add_persecutor(Targeter* tar);
	void remove_persecutor(Targeter* tar);
	flag_t target_type() const {return t_type; };

	virtual const std::string* get_name_ref() const {return &name; };
	virtual const std::string& get_name() const {return name; };
};

class Damageable { //Planeswalkers, Creatures and Players
protected:
	int life;
	TriggerEvent is_damaged;
public:
	explicit Damageable(int lif): life(lif){};
	Damageable(int lif, Card* source);
	int get_life() const { return life; };
	virtual ~Damageable() = default;
	virtual void damage(int nb_damage, Target* origin) = 0;
	virtual void set_life(int life_total){life = life_total; }
	virtual Player* getController() = 0;
};

template<typename T>
class OptionWrapper {

};

#endif //OLYMPUS_GENERALS_1_H
