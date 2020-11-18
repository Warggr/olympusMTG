#ifndef OLYMPUS_GENERALS_10_H
#define OLYMPUS_GENERALS_10_H

class Trigger{
private:
	int id_arg1;
	int id_arg2;
	int id_arg3;
	Ability* abi;
public:
	Trigger* next;
};

class Target{ //Permanents, Players, Cards, and Resolvables
protected:
	ModListNode* first_to_target;
	char target_flags; //hexproof
public:
	Target(): first_to_target(0){};
	virtual ~Target();
	void add_persecutor(Targeter* tar);
	void remove_persecutor(Targeter* tar);
	virtual char target_type() = 0;
};

class Damageable: virtual public Target{ //Planeswalkers, Creatures and Players
protected:
	int life;
	Trigger is_damaged;
public:
	Damageable(){};
	Damageable(int lif): life(lif){};
	int get_life(){return life; };
	virtual ~Damageable(){};
	virtual void damage(int nb_damage){life -= nb_damage; };
};

#endif //OLYMPUS_GENERALS_10_H