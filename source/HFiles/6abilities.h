#ifndef OLYMPUS_CLASSES_ABILITIES_6_H
#define OLYMPUS_CLASSES_ABILITIES_6_H

/*ALL POSSIBLE ABILITIES
0 deal damage (Damageable target, unsigned int nb_dam)
1 draw (Player target, int nb_cards)
2 gain/lose life (Player target, int nb_life)
3 set life to (Player target, int nb_life)
4 add mana(Player target, int mana)
5 destroy (Permanent target)
6 counter (Resolvable target) //we could add sth like (Resolvable target, Mana unless_pay_amount)
+X/+0 (Creature target, int x)
+0/+X (Creature target, int x)
destroy_all(characteristics)
exile (Permanent target)
copy (Resolvable target)
deal damage to all (characterstics, int nb_damage)
create token (Permanent to_copy)
//okay, so for tokens, a blank token will be created, then either an object will be copied or stats/abilities will be given.
make creature ?/X
make creature X/?
make player sacrifice
search library for a (characteristics)
make fight (Creature 1, Creature 2)
swap life (Player 1, Player 2)
gain control of (Player target)
*/

//each ability has zero to two targets. Multiple-target abilities such as (creature, +X/+Y) will be written as +X/+0 and +0/+Y separately

Ability* read_abs(FILE* myfile);

class PreResolvable{ //a Resolvable is a stack object with a list of targets and abilities.
	//a Pre-resolvable would be a printed instruction such as "X fights Y, and you gain 3 life", for which targets haven't been chosen.
	//Cards and Triggers contain Pre-resolvables, which can then be cast into Resolvables by choosing targets.
	std::unique_ptr<Ability> fab; //every Ability is actually subordinate to a PreResolvable, which is the object which might bring this Ability on the stack.
	char nb_parameters;
	char* parameters;
public:
	PreResolvable(): nb_parameters(0), parameters(0){};
	bool init(std::ifstream& myfile);
	std::string describe(std::string known_sourcename) const;
	void disp(int y, int z, int ySize, int zSize, std::string origin_name) const; //mimicks a Resolvable on top of the stack
	const Ability* getFab() const {return &(*fab); }; //i know, it's dangerous and it destroys the sense of having smart pointers. But hey, it makes more sense than a shared ptr
	char getNbParams() const {return nb_parameters; };
	const char* getParams() const {return parameters; };
};

class Ability{
protected:
	char type;
	char param1; //0 is 'you'. x+1 is 'the target number x in the parameter list of the Resolvable'
	char param2; //or literal x; depending on the exact ability
	std::unique_ptr<Ability> next;
public:
	Ability(std::unique_ptr<Ability>& a, char p1, char t, char p2): type(t), param1(p1), param2(p2), next(std::move(a)){};
	std::string describe(std::string cardname) const;
	void activate(Targeter* list_of_targets, Player* ctrl, Target* origin) const;
	void set_param(char a, char b){if(a==0) param1 = b; else param2 = b; };
	void set_type(char t){type = t; };
};

#endif //OLYMPUS_CLASSES_ABILITIES_6_H