#ifndef OLYMPUS_CLASSES_H
#define OLYMPUS_CLASSES_H

#define Olympus_VERSION_MAJOR 1
#define Olympus_VERSION_MINOR 0
#define IO_ALLEGRO

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

class Ability; //Represents a printed sentence such as "deal 3 damage", "Choose a creature and sacrifice it" or whatever. No targets chosen
//PreResolvables will contain Abilities and their necessary target list
class PreResolvable;

class Abstract_ui;
class Abstract_io;
class _UIElement;

typedef _UIElement UIElement;

class Dictionary;

#ifndef MANUAL_IMPORT_OF_OLYMPUS_HEADERS
#include "head1_constants.h"
#include "head2_mana.h"
#include "head3_readfiles.h"
#include "1general.h"
#include "2cards.h"
#include "3game.h"
#include "4board.h"
#include "5resolvables.h"
#include "6abstractIO.h"
#include "7abilities.h" //contains PreResolvable which is kinda important and needed by Options
#include "8options.h"
#include "9modifs.h"
#include "10triggers.h"
#include "11implementIO.h"
#endif

#endif //OLYMPUS_CLASSES_H