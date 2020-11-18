#ifndef OLYMPUS_CLASSES_H
#define OLYMPUS_CLASSES_H

#define LOGLEN 7
#define IO_X 400
#define LEFTBAR_Y 500

#define STACK_X 200
#define FULLCARD_X 523
#define RIGHTBAR_Y 375

#define HEADER_X 200
#define BOARD_X 300
#define BOARD_Y 500

#define CARD_X 80
#define CARD_Y 80

#include <cstdio>
#include <string>
#include <fstream>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_ttf.h>

#define NB_DIFFERENT_ABILITIES 7
#define ALL_DIFFERENT_ABILITIES {"ping", "draw", "+life", "set_life", "add", "destroy", "counter"}

typedef int Mana; //0xXcwubrg1, in inverse order of priority

class Targeter; //Represents an ability which has a target to affect, such as an enchantment's ability to Detain a creature, 
//or an Instant (on the stack)'s ability which will damage your opponent.
//Is (always, unless there are really weird exceptions) subordinate to a Spell or Permanent.
struct ModListNode; //We will use two types of lists of Modifs; output lists (all targets of a Spell or all creatures affected by an ability; 
//length mostly constant); and input lists (all Modifs affecting a certain Targetable, which will all become invalid if the creature disappears or gains protection)

class Target; //general class from which kinda every game object inherits. We could even make a hugely general class GameObject:
//is abstract supertype of Player, Permanent, Resolvable, Emblem

class Game;
class Player;

class Card; //THE FIRST RULE OF OLYMPUS: Cards are SACRED. Cards represent a printed piece of paper, 
//and will be the reference for creating and initializing permanents, options or spells.
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

//class Event;
class Ability; //Represents a printed sentence such as "deal 3 damage", "Choose a creature and sacrifice it" or whatever. No targets chosen
//Resolvables will contain Abilities and their necessary targets

struct ImageHolder;

struct externVarContainer{
	ImageHolder* images;
	Game* game;
	ALLEGRO_FONT* font;
	ALLEGRO_TIMER* timer;
	ALLEGRO_EVENT_QUEUE* queue;
	ALLEGRO_DISPLAY* window;
	std::ofstream debug_log;
	std::ofstream verbose_debug;
	char wanted_debug;

	externVarContainer();
	std::ofstream& gdebug(char password);
	void minimalKill();
};

extern struct externVarContainer god;

#include "classes_general.h"
#include "classes_cards.h"
#include "classes_options.h"
#include "classes_resolvables.h"
#include "classes_game.h"
#include "classes_board.h"
#include "header_mana.h"
#include "classes_abilities.h"
#include "classes_modifs.h"
#include "header_readfiles.h"
#include "header_triggers.h"
#include "header_images.h"

#endif //OLYMPUS_CLASSES_H