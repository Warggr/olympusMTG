#ifndef OLYMPUS_CLASSES_H
#define OLYMPUS_CLASSES_H

#define Olympus_VERSION_MAJOR 
#define Olympus_VERSION_MINOR 
/* #undef IO_ALLEGRO */

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
template <typename T> class PermanentTN;
class APPermanentN;

template <typename T> using PContainer = CollectionTN<T>;

class Ability; //Represents a printed sentence such as "deal 3 damage", "Choose a creature and sacrifice it" or whatever. No targets chosen
class PreResolvable; //PreResolvables will contain Abilities and their necessary target list

class Abstract_ui;
class Abstract_io;
class _UIElement;
typedef _UIElement UIElement;

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

struct Rect{
	uint16_t y, z;
	uint16_t width, height;

	int zone() const {return y >> 11; }
	int yy() const {return y & 0x07ff; }
	int right() const {return y + width; }
	int bottom() const {return z+height; }

	Rect() = default;
	Rect(int _y, int _z, int _width, int _height): y((int) _y), z((int) _z), width((int) _width), height((int) _height){};
	void shift(int dy, int dz){y += dy; z += dz; }
};

extern struct externVarContainer god;

typedef int Identifier; //ttttt o ccccc ssss 0000 0000 0000 0000, in inverse order!
//(permanent?) object type - owner - color - subtypes - is_an_artifact bit - is_an_enchantment bit - CMC - specific stuff

inline bool fulfills(Identifier chars, Identifier requs, Identifier test){
	return (((chars^test)&requs) == 0);
}

#define DBG_IMPORTANT 0x1
#define DBG_READFILE 0x2
#define DBG_X_READFILE 0x4
#define DBG_INTERNAL 0x8
#define DBG_TARGETING 0x10
#define DBG_RAGNAROK 0x20
#define DBG_IOUI 0x40

enum DirectioL{ UP, DOWN, LEFT, RIGHT, BACK, ENTER, MOUSE, NOT_RECOGNIZED};

[[noreturn]] void raise_error(const std::string& message);

#endif //OLYMPUS_CLASSES_H
