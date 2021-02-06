#ifndef OLYMPUS_CLASSES_H
#define OLYMPUS_CLASSES_H

#define Olympus_VERSION_MAJOR @Olympus_VERSION_MAJOR@
#define Olympus_VERSION_MINOR @Olympus_VERSION_MINOR@
#cmakedefine IO_ALLEGRO

#define LOGLEN 7

#include <fstream>
#include <string>
#include <list>

//A great number of theses objects belong to Cards and cannot be modified. They'll be marked as holy.

class Mana;

class Targeter;
class Target; //general class from which kinda every game object inherits. We could even make a hugely general class GameObject:
//is abstract supertype of Player, Permanent, Resolvable, Emblem

class Game;
class Player;

struct Phase;

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

class Trigger;
class TriggerEvent;

class StaticAb;
struct ModifListNode;
class BoardN;
class AbstractN;
template <typename T> class CollectionTN;
template <typename T> class DefaultCollectionTN;
template <typename T> class StatedCollectionTN;
template <typename T> class PermanentTN;
class AbstractPermanentN;

template <typename T>
using PContainer = CollectionTN<T>;

template<typename T> class typediterator;
template<typename T> class c_iterator;

class Ability; //Represents a printed sentence such as "deal 3 damage", "Choose a creature and sacrifice it" or whatever. No targets chosen
//PreResolvables will contain Abilities and their necessary target list
class PreResolvable;

class Abstract_ui;
class Abstract_io;
class _UIElement;

typedef _UIElement UIElement;

enum DirectioL{ UP, DOWN, LEFT, RIGHT, BACK, ENTER, MOUSE, NOT_RECOGNIZED};

struct externVarContainer{
	Game* game;
	std::ofstream debug_log, verbose_debug;
	char wanted_debug;
	Abstract_ui* myUI;
	Abstract_io* myIO;

	externVarContainer();
	std::ofstream& gdebug(char password);
	void call_ragnarok();
	void initialize(Game* gm, char debug_flags);
};

extern struct externVarContainer god;

typedef int Identifier; //ttttt o ccccc ssss 0000 0000 0000 0000, in inverse order!
//(permanent?) object type - owner - color - subtypes - is_an_artifact bit - is_an_enchantment bit - CMC - specific stuff

inline bool fulfills(Identifier chars, Identifier requs, Identifier test){
	return (((chars^test)&requs) == 0);
}

#ifndef IMPORT_ONLY_DECLARATIONS
#include "1general.h"
#ifndef MANUAL_IMPORT_OF_OLYMPUS_HEADERS
#include "head1_constants.h"
#include "2cards.h"
#include "3player.h"
#include "4permanents.h"
#include "5resolvables.h"
#include "6abstractIO.h"
#include "../HFiles/12abilities.h" //contains PreResolvable which is kinda important and needed by Options
#include "7game.h"
#endif
#endif

#define DBG_IMPORTANT 0x1
#define DBG_READFILE 0x2
#define DBG_X_READFILE 0x4
#define DBG_INTERNAL 0x8
#define DBG_TARGETING 0x10
#define DBG_RAGNAROK 0x20
#define DBG_IOUI 0x40

[[noreturn]] void raise_error(const std::string& message);

#endif //OLYMPUS_CLASSES_H