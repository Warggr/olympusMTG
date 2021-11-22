#ifndef OLYMPUS_CLASSES_RESOLVABLE_5_H
#define OLYMPUS_CLASSES_RESOLVABLE_5_H

#include "classes/1effects.h"
#include "gameplay/1general.h"
#include <string>
#include <forward_list>
#include <memory>
class Player; class Game;

/* A Resolvable is any object that can be put on the stack and resolve. It can be:
a StackAbility (e.g. an activated ability of a permanent). Will have effects upon resolve and disappear.
a Spell: has a source, which is a Card. Can have on_cast triggers (such as "When you cast this spell copy it for...")
    in addition to its resolvable abilities (create 'Permanent' object, or do some stuff and put the card to the myGraveyard)
    The on_cast will be a StackAbility, as it has no card source and can't have on_cast abilities itself (it is not 'cast').
There is technically nothing that prevents StackAbilities to have on_cast triggers...
We could imagine sth like "do blah. If you have this and that, copy this ability when you activate it"
*/

class Resolvable: public Target {
protected:
	Player* ctrl;
	std::unique_ptr<Targeter> origin; //the one every 'this' refers to, such as 'when this exploits a creature', 'this deals damage', ...
	char nb_targets;
	Targeter* list_of_targets;
	const Effect_H* on_resolve; //we can't change the Ability, since it's a Card characteristics
	static const std::string description;
public:
    Resolvable(Player* ct, const Effect_H* preRes, Target* origin = nullptr);
	virtual ~Resolvable() = default;
	virtual void resolve(); //this is what a resolvable is about
	virtual std::string describe() const override { return description; };
    virtual void disp(BasicIO* io) const override;
	virtual void counter();

    Player* getController() override { return ctrl; }
};

class Spell: public Resolvable{
private:
	std::unique_ptr<Card> source; //the source will be kept by the option, the spell and the permanent to finally go the myGraveyard
public:
	Spell(std::unique_ptr<Card> src, Player* ct);
	void resolve() override;
	void counter() override;
	std::string describe() const override;

    void disp(BasicIO* io) const override;
};

#endif //OLYMPUS_CLASSES_RESOLVABLE_5_H