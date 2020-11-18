#ifndef OLYMPUS_CLASSES_RESOLVABLE_5_H
#define OLYMPUS_CLASSES_RESOLVABLE_5_H

//okay lets forget about on_cast for the moment because it requires triggered abilities to be implemented.
/* A Resolvable is any object that can be put on the stack and resolve. It can be:
a StackAbility (e.g. an activated ability of a permanent). Will have effects upon resolve and disappear.
a Spell: has a source, which is a Card. Can have on_cast triggers (such as "When you cast this spell copy it for...")
    in addition to its resolvable abilities (create 'Permanent' object, or do some stuff and put the card to the graveyard)
    The on_cast will be a StackAbility, as it has no card source and can't have on_cast abilities itself (it is not 'cast').
There is technically nothing that prevents StackAbilities to have on_cast triggers...
We could imagine sth like "do blah. If you have this and that, copy this ability when you activate it"
*/

class Resolvable: virtual public Target{
protected:
	Player* ctrl;
	Targeter* list_of_targets;
	char nb_targets;
	Ability* on_resolve; //for permanent spells, this will be used for on_ETB abilities
public:
	Resolvable* next;

	Resolvable(Player* ct, int nb_targets, Ability* fab, char* paramtypelist);
	virtual ~Resolvable();
	virtual void resolve(); //this is what a resolvable is about
	virtual std::string describe(){return std::string(); };
	virtual void counter(Game* metagame);
	char target_type(){return 0x20; };
};

class Spell: public Resolvable{
private:
	Card* source; //the source will be kept by the option, the spell and the permanent to finally go the graveyard
public:
	Spell(Card* src, Player* ct);
	void resolve();
	void counter(Game* metagame);
	std::string describe(){return source->describe(); };
};

#endif //OLYMPUS_CLASSES_RESOLVABLE_5_H