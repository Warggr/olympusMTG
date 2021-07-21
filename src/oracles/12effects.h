#ifndef OLYMPUS_CLASSES_ABILITIES_12_H
#define OLYMPUS_CLASSES_ABILITIES_12_H

//each ability has zero to two targets. Multiple-target abilities such as (creature, +X/+Y) will be written as +X/+0 and +0/+Y separately

#include "1general.h"
#include "HFiles/headI_identifiers.h"
#include "headQ_rect.h"
#include "headE_enums.h"
#include <memory>
class Player; class Target; class Permanent; class FileReader;

class AtomEffect_H { //TODO logical gate-like effects, such as "do this or do that". Or modal spells.
public:
	typedef effect_type type_t;
protected:
	type_t type;
	char params[3]; //0 is 'you'. x+1 is 'the target number x in the parameter list of the Resolvable' or literal x; depending on the exact ability
	std::unique_ptr<AtomEffect> next;
public:
	AtomEffect_H(FileReader& reader, std::unique_ptr<AtomEffect>& a, char* allassignedparams, uint8_t& nbassignedparams);
	AtomEffect_H(std::ifstream& bFile);
	std::string describe(std::string cardname) const;
	void activate(Targeter* list_of_targets, Player* ctrl, Target* origin) const;
	void mana_activate(Player* ctrl, Target* origin) const;
	void write_binary(std::ofstream& bFile) const;
};

class Effect_H{ //a Resolvable is a stack object with a list of targets and abilities.
	//a Pre-resolvable would be a printed instruction such as "X fights Y, and you gain 3 life", for which targets haven't been chosen.
	//Cards and Triggers contain Pre-resolvables, which can then be cast into Resolvables by choosing targets.
	std::unique_ptr<AtomEffect> fab; //the Effect_H is the object which might bring its children AtomEffects on the stack.
	uint8_t nb_parameters;
	char* parameters;
public:
	Effect_H(){};
	Effect_H(FileReader& reader){ init(reader); }
	Effect_H(std::ifstream& bFile);
	Effect_H(Effect_H&& other): nb_parameters(other.nb_parameters), parameters(other.parameters){ fab = std::move(other.fab); }
	void init(FileReader& reader);
	void init(std::ifstream& bFile);
	void write_binary(std::ofstream& bFile) const;
	std::string describe(std::string known_sourcename) const;
	void disp(const Rect& zone, std::string origin_name) const; //mimicks a Resolvable on top of the stack
	AtomEffect* getFab() const {return &(*fab); }; //i know, it's dangerous and it destroys the sense of having smart pointers. But hey, it makes more sense than a shared ptr
	uint8_t getNbParams() const {return nb_parameters; }
	const char* getParams() const {return parameters; }
	void straight_cast(Player* pl, Permanent* origin);
};

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
gain control of (Player target) */

#endif //OLYMPUS_CLASSES_ABILITIES_12_H
