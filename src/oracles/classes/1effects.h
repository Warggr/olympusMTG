#ifndef OLYMPUS_CLASSES_EFFECTS_1_H
#define OLYMPUS_CLASSES_EFFECTS_1_H

//each ability has zero to two targets. Multiple-target abilities such as (creature, +X/+Y) will be written as +X/+0 and +0/+Y separately

#include "headE_enums.h"
#include <string>
#include <forward_list>
class Player; class Target; class Permanent; class ReaderVisitor; class Targeter; class Rect;

class AtomEffect_H { //TODO logical gate-like effects, such as "do this or do that". Or modal spells.
public:
	typedef effect_type type_t;
protected:
	type_t type;
	flag_t* params; //0 is 'you'. x+1 is 'the target number x in the parameter list of the Resolvable'
	// or literal x; depending on the exact ability
public:
	AtomEffect_H(ReaderVisitor& reader, char* params_hashtable, unsigned char& effects_param) {
	    init(reader, params_hashtable, effects_param);
	}
	void init(ReaderVisitor& reader, char* allassignedparams, unsigned char& nbassignedparams);
	~AtomEffect_H() { delete[] params; }
	std::string describe(const std::string& cardname) const;
    void write_binary(std::ofstream& bFile) const;

	void activate(Targeter* list_of_targets, Player* ctrl, Target* origin) const;
	void mana_activate(Player* ctrl, Target* origin) const;
};

class Effect_H { //an Effect_H is a printed instruction such as "X fights Y, and you gain 3 life",
    // for which targets haven't been chosen. They can be cast into Resolvables by choosing targets.
	std::forward_list<AtomEffect_H> effects;
	unsigned char nb_parameters;
	char* parameters;
public:
	Effect_H() = default;
	Effect_H(ReaderVisitor& reader){ init(reader); }
	Effect_H(Effect_H&& other): nb_parameters(other.nb_parameters), parameters(other.parameters) { effects = std::move(effects); }
	void init(ReaderVisitor& reader);
	void write_binary(std::ofstream& bFile) const;
	std::string describe(std::string known_sourcename) const;
	uint8_t getNbParams() const { return nb_parameters; }
	const char* getParams() const { return parameters; }

    void straight_cast(Player* pl, Permanent* origin);
    void activate(Targeter* list_of_targets, Player* ctrl, Target* origin) const;

    void disp(const Rect& zone, std::string origin_name) const; //mimicks a Resolvable on top of the stack
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
make local sacrifice
search library for a (characteristics)
make fight (Creature 1, Creature 2)
swap life (Player 1, Player 2)
gain control of (Player target) */

#endif //OLYMPUS_CLASSES_EFFECTS_1_H
