#ifndef OLYMPUS_CLASSES_H
#define OLYMPUS_CLASSES_H

#include <fstream>
#include <iostream>
#include <vector>
#include <forward_list>
#include <list>
#include <algorithm>
#include <memory>

#define LOGLEN 7

//A great number of theses objects belong to Cards and cannot be modified. They'll be marked as holy.

typedef int Mana; //0xXcwubrg1, in inverse order of priority

class Targeter; //Represents an ability which has a target to affect, such as an enchantment's ability to Detain a creature, 
//or an Instant (on the stack)'s ability which will damage your opponent.
//Is (always, unless there are really weird exceptions) subordinate to a Spell or Permanent.
//We will use two types of lists of Modifs; output lists (all targets of a Spell or all creatures affected by an ability; 
//length mostly constant); and input lists (all Modifs affecting a certain Targetable, which will all become invalid if the creature disappears or gains protection)

class Target; //general class from which kinda every game object inherits. We could even make a hugely general class GameObject:
//is abstract supertype of Player, Permanent, Resolvable, Emblem

class Game;
class Player;

struct Phase;

//THE FIRST RULE OF OLYMPUS: Cards are SACRED. Cards represent a printed piece of paper, 
//and will be the reference for creating and initializing permanents, options or spells.
class CardOracle;
class Card;
class CardZone;

class Permanent;
class Creature;
class Artifact;
class Land;
class Planeswalker;

class Option; //these options are in your hand
class SpellOption;
class PlayLand;
class PermOption;

class Resolvable; //similar to option, except it is on the stack and parameters have already been chosen.
class Spell;
//class PermAbility;

class Trigger;
class TriggerEvent;

class Ability; //Represents a printed sentence such as "deal 3 damage", "Choose a creature and sacrifice it" or whatever. No targets chosen
//PreResolvables will contain Abilities and their necessary target list
class PreResolvable;

class Abstract_ui;
class Abstract_io;
class _UIElement;

typedef class _UIElement UIElement;

class Dictionary;

struct externVarContainer{
	Game* game;
	std::ofstream debug_log;
	std::ofstream verbose_debug;
	char wanted_debug;
	Abstract_ui* myUI;
	Abstract_io* myIO;

	externVarContainer();
	std::ofstream& gdebug(char password);
	void minimalKill();
};

extern struct externVarContainer god;

#include "head1_constants.h"
#include "head2_mana.h"
#include "head3_readfiles.h"
#include "1general.h"
#include "2cards.h"
#include "3modifs.h" //requires Target from General
#include "4game.h" //Player contains CardZone, CardZone contains Card
#include "5board.h" //requires Targeter for some inline operations
#include "6abilities.h"
#include "7options.h"
#include "8resolvables.h"
#include "9abstractIo.h"
#include "10triggers.h"
#include "11abstractUI.h"

#endif //OLYMPUS_CLASSES_H