#ifndef OLYMPUS_GENERALS_1_H
#define OLYMPUS_GENERALS_1_H

#include "9modifs.h"
#include "headE_enums.h"
#include "displayable.h"
#include <forward_list>
#include <string>
#include <list>

using Identifier = int;

class Player; class Target; class Effect_H; class Card;

class TriggerEvent{ //contains all X-action triggers of a certain object, i.e. all Death triggers of one permanent.
	//a Trigger contains 3 constraints about the actions that might trigger him: whenever <a Beeble> deals <combat damage> to <a Brushwagg>, ...
	std::forward_list<Effect_H*> all_triggers;
	int id_args[3];
public:
	void trigger(Player* pl, Target* origin) const;
	void addTrigger(Effect_H* trig){ all_triggers.push_front(trig); };
	std::string describe(trig_type type, const std::string& cardname) const;
};

class GameObject{ //Players, Permanents, Resolvables, Cards, and maybe enums or zones or abilities (as in: abilities can't be activated) later on? (Or, would that restriction be put on the origin, or the resolvable?)
private:
    Identifier identifier;
    bool id_still_correct;
public:
    GameObject(): identifier(0), id_still_correct(false) {};
    void invalidate(){ id_still_correct = false; };
};

class Target: public GameObject, virtual public Displayable { //Permanents, Players, Cards (not Oracles!), and Resolvables
protected:
	std::list<AbstractTargeter*> to_target;
	flag_t t_type{0};
	char target_flags; //hexproof
	const std::string& name;
public:
    TriggerEvent deals_damage;

	explicit Target(const std::string& name): target_flags(0), name(name) {};
	//Target(SameObjectTN* arch, char type, std::string name): GameObject(arch), t_type(type), target_flags(0), name(std::move(name)) {};
	virtual ~Target();
	void addPersecutor(AbstractTargeter* tar);
	void removePersecutor(AbstractTargeter* tar);
	flag_t targetType() const { return t_type; };

	virtual Player* getController() = 0;
	virtual const std::string* getNameRef() const {return &name; };
	virtual const std::string& getName() const {return name; };
};

class Damageable { //Planeswalkers, Creatures and Players
protected:
	int life;
public:
    TriggerEvent is_damaged;

	explicit Damageable(int lif): life(lif){};
	Damageable(int lif, Card* source);
	int getLife() const { return life; };
	virtual ~Damageable() = default;
	virtual void damage(int nb_damage, Target* origin);
	virtual void setLife(int life_total){life = life_total; }
	virtual Player* getDmgController() = 0; //Not const because Player returns a non-const reference to itself
	virtual Target* getMeAsTarget() = 0;
};

#endif //OLYMPUS_GENERALS_1_H
