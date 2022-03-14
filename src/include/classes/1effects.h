#ifndef OLYMPUS_CLASSES_EFFECTS_1_H
#define OLYMPUS_CLASSES_EFFECTS_1_H

//each ability has zero to two targets. Multiple-target abilities such as (creature, +X/+Y) will be written as +X/+0 and +0/+Y separately

#include "headE_enums.h"
#include "serializable.h"
#include <string>
#include <forward_list>
class Player; class Target; class Permanent; template<typename T> class SpecificTargeter; class Rect;

class AtomEffect_H { //TODO FEATURE logical gate-like effects, such as "do this or do that". Or modal spells.
public:
	typedef effect_type type_t;
protected:
	type_t type;
	flag_t* params; //0 is 'you'. x+1 is 'the target number x in the parameter list of the Resolvable'
	// or literal x; depending on the exact ability
public:
	AtomEffect_H(type_t type, flag_t* params): type(type), params(params) {  };
    ~AtomEffect_H() { delete[] params; }

	void activate(SpecificTargeter<Target>* list_of_targets, Player* ctrl, Target* origin) const;

    type_t getType() const { return type; }
    flag_t* getParams() const { return params; }
    std::string describe(const std::string& cardname) const;
    template<bool read> friend void visit(ConstHost<AtomEffect_H, read>&, Visitor<read>&);
};

/** A printed instruction such as "X fights Y, and you gain 3 life",
* for which targets haven't been chosen. They can be cast into Resolvables by choosing targets. */
class Effect_H {
	std::forward_list<AtomEffect_H> effects;
	unsigned char nb_parameters;
	char* parameters;
public:
	Effect_H() = default;
	Effect_H(ReaderVisitor& reader);
	Effect_H(Effect_H&& other) noexcept : nb_parameters(other.nb_parameters), parameters(other.parameters) {
	    effects = std::move(other.effects);
	}
	~Effect_H() { delete parameters; }

    void activate(SpecificTargeter<Target>* list_of_targets, Player* ctrl, Target* origin) const;

	std::string describe(const std::string& known_sourcename) const;

	uint8_t getNbParams() const { return nb_parameters; }
	const char* getParams() const { return parameters; }

//    void straight_cast(Player* pl, Permanent* origin);
//    void disp(const Rect& zone, std::string origin_name) const; //mimicks a Resolvable on top of the stack
    template<bool read> friend void visit(ConstHost<Effect_H, read>&, Visitor<read>&);
	friend class PlainFileReader;
};

#endif //OLYMPUS_CLASSES_EFFECTS_1_H
