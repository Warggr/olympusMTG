#ifndef OLYMPUS_CLASSES_MODIFS_3_H
#define OLYMPUS_CLASSES_MODIFS_3_H

//Represents an ability which has a target to affect, such as an enchantment's ability to Detain a creature, 
//or an Instant (on the stack)'s ability which will damage your opponent.
//Is (always, unless there are really weird exceptions) subordinate to a Spell or Permanent.
//We will use two types of lists of Modifs; output lists (all targets of a Spell or all creatures affected by an ability; length mostly constant);
//and input lists (all Modifs affecting a certain Targetable, which will all become invalid if the creature disappears or gains protection)
//In short: Resolvables contains (an array of) Targeters, and Targets contain (a list of) Targeter pointers
//Additionally, Triggers contain an Origin targeter, which is little more than a pointer. such targeters are called 'interenal targeters' and have actually no game relevance

struct Targeter{
private:
	Target* content;
	std::string name; //will be remembered even after the target dies
public:
	bool valid;
	char colorIdentity; //-1 means it's an "internal" targeter, such as the one linking an ability to its origin.
	//you can't, of course, have protection from 'internal' targeters

	Targeter(Target* tar): valid(true) {setTarget(tar); };
	Targeter(): valid(true) {god.gdebug(DBG_TARGETING) << " Creating a Targeter\n"; };
	~Targeter(); //removes references to itself from its victim
	void setTarget(Target* tgt);
	Target* getTarget() const {if(valid) return content; else return 0; };
	std::string get_name() const {return name; };
};

#endif //OLYMPUS_CLASSES_MODIFS_3_H