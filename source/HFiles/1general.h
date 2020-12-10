#ifndef OLYMPUS_GENERALS_10_H
#define OLYMPUS_GENERALS_10_H

class TriggerEvent{ //contains all X-action triggers of a certain object, i.e. all Death triggers of one permanent.
	//a Trigger contains 3 constraints about the actions that might trigger him: whenever <a Beeble> deals <combat damage> to <a Brushwagg>, ...
	std::forward_list<Trigger*> all_triggers;
	int id_arg1;
	int id_arg2;
	int id_arg3;
public:
	void trigger(Player* pl, Targeter* origin) const;
	void add_trigger(Trigger* trig){all_triggers.push_front(trig); };
	std::string describe(int typeoftrig, std::string cardname) const;
};

class Target{ //Permanents, Players, Cards, and Resolvables
protected:
	std::list<Targeter*> to_target;
	char target_flags; //hexproof
public:
	Target(): to_target(0){};
	virtual ~Target();
	virtual const std::string& get_name() const = 0; //TODO: name is an attribute of the targeter, much as target_flags
	void add_persecutor(Targeter* tar);
	void remove_persecutor(Targeter* tar);
	char target_type() const {return target_flags; };
};

class Damageable: virtual public Target{ //Planeswalkers, Creatures and Players
protected:
	int life;
	TriggerEvent is_damaged;
public:
	Damageable(int lif): life(lif){};
	Damageable(int lif, Card* source);
	int get_life(){return life; };
	virtual ~Damageable(){};
	virtual void damage(int nb_damage, Target* origin = 0) = 0;
	virtual void set_life(int life_total){life = life_total; };
};

#endif //OLYMPUS_GENERALS_10_H