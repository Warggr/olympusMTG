#ifndef OLYMPUS_6_ABSTRACT_IO
#define OLYMPUS_6_ABSTRACT_IO

#include <vector>
#include <string>
#include "../Mana/lib2_mana.h"

class Abstract_io{
public:
	static constexpr uint8_t HIGH1=0x80, HIGH2=0x40, BLACK = 0x30, COLORLESS = 0x20, WHITE = 0x10;
	virtual ~Abstract_io(){};
	virtual void getResolution(int& YSize, int& ZSize, bool& hasMouseSupport, int& linesize) const = 0;
	virtual void harmonize(const Rect& poster, const Rect& message, int nb_winzones) = 0;
	virtual void draw_rectangle(char color, const Rect& rectangle, int linewidth) const = 0;
	virtual void draw_full_rectangle(char color, const Rect& rectangle) const = 0;
	virtual void erase_surface(const Rect& rectangle) const = 0;
	virtual void print_text(const std::string text, char color, int y, int z) const = 0;
	virtual void print_text(const char* text, char color, int y, int z) const = 0;
	virtual void draw_boxed_text(const char* text, char color, char backgr_color, int left, int top, int width) const = 0;
	virtual void draw_boxed_text(const std::string text, char color, char backgr_color, int left, int top, int width) const = 0;
	virtual void message(const char* message) const = 0;
	virtual void message(const std::string message) const = 0;
	virtual void fulldisp() const = 0;
	virtual void draw_permanent(const Rect& zone, char color, bool tapped, bool highlight, bool basicImg) const = 0;
	virtual void disp_header(const Rect& zone, const char* name, int life, char phase, bool highlight, Mana pool) const = 0;
	virtual void poster(const std::string name, Mana manacost, char color, const char* types,
		const std::vector<std::string> lines, int power, int toughness, char frametype, bool watermark) const = 0;
	virtual void disp_cardback(const Rect& zone, int oncard_number) const = 0;
	virtual DirectioL get_direction_key() = 0;
	virtual int getInt(int lowerBound, int upperBound) = 0;
	virtual float gmouseY() = 0;
	virtual float gmouseZ() = 0;
	virtual bool gmouseActive() = 0;
	virtual bool attack_switch(int leftY, int rightY, int topZ, int arrowlength) const = 0;
	virtual void declare_window(int& leftY, int& topz, int width, int height) = 0;
};

class Abstract_ui{
protected:
	Abstract_io* myIO;
	bool mouseSupport;
public:
	static const int ELTYPE_STACK = 0, ELTYPE_LOGBOOK = 1, ELTYPE_OPTIONS = 2, ELTYPE_PERMANENTS = 3;

	Abstract_ui(Abstract_io* IO): myIO(IO){};
	virtual ~Abstract_ui() = default;
	virtual UIElement* declare_element(int typeofelement, char player_id) = 0;

	virtual void get_player_coords(char player_id, Rect* zone, int* liby, int* libz, int* gravey, int* gravez, int* exily, int* exilz, int* cardzoneY, int* cardzoneZ) = 0;
	//different zones like the stack, the battlefield, the hand a.s.o. will be handled by team members aka _UIElements
	//while displaying the player is the personal responsibility of the UI
	//virtual void resize_zones(); //in later versions, zone might change position and size to occupy the whole space, as in MTGArena.
	virtual bool chooseattackers(PContainer<Creature>& cowards, PContainer<Creature>& warriors, char player_id) = 0;
	virtual void chooseblockers(PContainer<Creature>& defenders, PContainer<Creature>& attackers, UIElement* defenderDisplay, UIElement* attackerDisplay) = 0;
	virtual void clear_opts() = 0;
	virtual UIElement* get_optionzone() = 0;
	virtual Target* iterate(bool needstarget, Player** pl, char returntypeflags) = 0;
	virtual Option* choose_opt(bool sorceryspeed, Option* firstopt, Player* asker, int metapos) = 0;
	virtual void deadzone() = 0;
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
	Rect get_coordinates(int* yOff, int* zOff) const;
	void erase_background(Abstract_io* io) const;
	template <class T>
	void disp_all(const PContainer<T>* const objects) const{
		erase_background(god.myIO);
		Rect rect(leftY, topZ, ySize, zSize);
		for(auto iter = objects->cbegin(); iter != objects->cend(); iter++){
			iter->disp(rect, false);
			rect.shift(yOffset, zOffset);
		}
	}
};

typedef _UIElement UIElement;

Abstract_io* new_IOLib();
Abstract_ui* new_UILib(Abstract_io* IOLib);

inline char main_color(char color){
    switch(color){
        case 0: return 0;
        case 1: return 1;
        case 2: return 2;
        case 4: return 3;
        case 8: return 4;
        case 16: return 5;
        default: return 6;
    }
}

#endif //OLYMPUS_6_ABSTRACT_IO