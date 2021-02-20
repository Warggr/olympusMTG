#ifndef OLYMPUS_CLASSES_RESOLVABLE_5_H
#define OLYMPUS_CLASSES_RESOLVABLE_5_H

#include "1general.h"

//okay lets forget about on_cast for the moment because it requires triggered abilities to be implemented.
/* A Resolvable is any object that can be put on the stack and resolve. It can be:
a StackAbility (e.g. an activated ability of a permanent). Will have effects upon resolve and disappear.
a Spell: has a source, which is a Card. Can have on_cast triggers (such as "When you cast this spell copy it for...")
    in addition to its resolvable abilities (create 'Permanent' object, or do some stuff and put the card to the graveyard)
    The on_cast will be a StackAbility, as it has no card source and can't have on_cast abilities itself (it is not 'cast').
There is technically nothing that prevents StackAbilities to have on_cast triggers...
We could imagine sth like "do blah. If you have this and that, copy this ability when you activate it"
*/

class Resolvable: public Target{
protected:
	Player* ctrl;
	std::unique_ptr<Targeter> origin; //the one every 'this' refers to, such as 'when this exploits a creature', 'this deals damage', ...
	char nb_targets;
	Targeter* list_of_targets;
	const Ability* on_resolve; //we can't change the Ability, since it's a Card characteristics
	static const std::string description;
public:
    Resolvable(Player* ct, const PreResolvable* preRes, Target* origin = 0);
	virtual ~Resolvable();
	virtual void resolve(); //this is what a resolvable is about
	virtual std::string describe() const {return description; };
	virtual void disp(const Rect& zone, bool highlight = false) const;
	virtual void counter(Game* metagame);
	virtual const std::string& get_name() const {return description; };
	virtual Identifier reload_id() const;
};

class Spell: public Resolvable{
private:
	Card* source; //the source will be kept by the option, the spell and the permanent to finally go the graveyard
public:
	Spell(Card* src, Player* ct);
	void resolve();
	void counter(Game* metagame);
	std::string describe() const {return source->describe(); };
	const std::string& get_name() const {return source->get_name(); };
	virtual Identifier reload_id() const;
};

#endif //OLYMPUS_CLASSES_RESOLVABLE_5_H