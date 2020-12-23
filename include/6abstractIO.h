#ifndef OLYMPUS_6_ABSTRACT_IO
#define OLYMPUS_6_ABSTRACT_IO

#include <vector>
#include <string>
#include "../Mana/head2_mana.h"
#include "4board.h"

class Abstract_io{
protected:
	int posterY, posterZ;
	int messageY, messageZ;
public:
	static const int BLACK, WHITE, GREY, HIGH1, HIGH2;

	virtual ~Abstract_io(){};
	//virtual void getcoordinates(int* maxX, int* maxY, char* resolution) = 0;
	virtual void getResolution(int* posterYSize, int* posterZSize) = 0;
	virtual void harmonize(int _posterY, int _posterZ, int _messageY, int _messageZ){posterY = _posterY; posterZ = _posterZ; messageY = _messageY; messageZ = _messageZ; };

	virtual void fulldisp() = 0;
	virtual void draw_permanent(int left, int top, int width, int height, char color, bool tapped, bool highlight, bool basicImg) = 0;
	virtual void refresh_display() = 0;
	virtual void print_text(std::string const text, char color, int y, int z) = 0;
	virtual void disp_mana(Mana mana, int endy, int topz) = 0;
	virtual void draw_rectangle(char color, int y, int z, int dy, int dz, int linewidth) = 0;
	virtual bool attack_switch(int leftY, int rightY, int topZ, int arrowlength) = 0;
	virtual void draw_full_rectangle(char color, int left, int top, int width, int height) = 0;
	virtual void erase_surface(int left, int top, int width, int height) = 0;
	virtual void disp_header(int y, int z, int width, int height, const char* name, int life, char phase, bool highlight, Mana pool) = 0;
	virtual char get_direction_key() = 0;
	virtual int getInt(int lowerBound, int upperBound) = 0;

	virtual Creature* blocker_switch(const Creature& blocker, int blockerY, int blockerZ,
		std::list<Creature>& attackers, UIElement* attacker_io,
		int creatureWidth, int creatureDepth) = 0;
	virtual void poster(const std::string name, Mana manacost, char color, const char* types,
		const std::vector<std::string> lines, int power, int toughness, char frametype, bool watermark) = 0;
	virtual void disp_cardback(int y, int z) = 0;
	virtual void message(const char* message) = 0;
	virtual void message(std::string message) = 0;
};

class Abstract_ui{
protected:
	Abstract_io* myIO;
public:
	static const int ELTYPE_STACK = 0, ELTYPE_LOGBOOK = 1, ELTYPE_OPTIONS = 2, ELTYPE_PERMANENTS = 3;

	Abstract_ui(Abstract_io* IO): myIO(IO){};
	virtual ~Abstract_ui(){};
	virtual UIElement* declare_element(const int typeofelement, char player_id) = 0;

	virtual void get_player_coords(char player_id, int* py, int* pz, int* wide, int* high, int* liby, int* libz, int* gravey, int* gravez, int* exily, int* exilz) = 0;
	//different zones like the stack, the battlefield, the hand a.s.o. will be handled by team members aka _UIElements
	//while displaying the player is the personal responsibility of the UI
	//virtual void resize_zones(); //in later versions, zone might change position and size to occupy the whole space, as in MTGArena.
	virtual bool chooseattackers(std::list<Creature>& cowards, std::list<Creature>& warriors, char player_id) = 0;
	virtual void chooseblockers(std::list<Creature>& defenders, std::list<Creature>& attackers, UIElement* defenderDisplay, UIElement* attackerDisplay) = 0;
	virtual void clear_opts() = 0;
	virtual UIElement* get_optionzone() = 0;
	virtual Target* iterate(bool needstarget, Player** pl, char returntypeflags) = 0;
	virtual Option* choose_opt(float zOffset, bool sorceryspeed, Option* firstopt, Player* asker, int metapos) = 0;
	virtual void report_mouse_move(float x, float y) = 0;
};

class _UIElement{
private:
	int topZ, leftY;
	int yOffset, zOffset;
	int ySize, zSize;
	int maxItems;
public:
	static const bool horizontal = true;
	static const bool vertical = false;

	_UIElement(int left, int top, int ySize, int zSize, int offset, int maxItems, bool direction);
	void get_coordinates(int itemNb, int* y, int* z) const;
	void get_coordinates(int* y, int* z, int* yOffset, int* zOffset) const;
	void get_coordinates(int* y, int* z, int* yOff, int* zOff, int* yWidth, int* zHeight) const;
	void erase_background(Abstract_io* io) const;
	template <class T>
	void disp_all(const std::list<T>& objects) const{
		erase_background(god.myIO);
		int y = leftY;
		int z = topZ;
		for(auto iter = objects.begin(); iter != objects.end(); iter++){
			iter->disp(y, z, ySize, zSize, false);
			y += yOffset;
			z += zOffset;
		}
	}
};

typedef _UIElement UIElement;

Abstract_io* new_IOLib();
Abstract_ui* new_UILib(Abstract_io* IOLib);

#endif //OLYMPUS_6_ABSTRACT_IO