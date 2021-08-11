#ifndef OLYMPUS_11_IMPLEMEMT_UI
#define OLYMPUS_11_IMPLEMENT_UI

#include "build_types.h"
#include "../io/6abstractIO.h"
class Permanent; class Creature; class Player; class Target; class OptionAction; template<typename T> class PContainer;

struct UIElement {
    int topZ, leftY, yOffset, zOffset, ySize, zSize, maxItems;
    static constexpr bool vertical = true, horizontal = false;

    UIElement(int left, int top, int _ySize, int _zSize, int offset, int _maxItems, bool direction);

    Rect get_coordinates(int *yOff, int *zOff) const;
    void get_coordinates(int *y, int *z, int *yOff, int *zOff) const;
    void get_coordinates(int itemNb, int *y, int *z) const;
    void erase_background(AbstractIO *io) const;
};

class DefaultUI {
private:
    ImplementIO* myIO;
	int leftbarW, boardW, rightbarW;
	int stackH, optionsH, playerH, iozH, posterH;
	int permanentZSize, permanentYSize, permanentZMargin;
	int optionZSize;
	int playerY[2], playerZ[2];
	int gridy{0}, gridz{0};
	int linesize;
    bool mouseSupport;

	DirectioL direction;
	UIElement* playerPerms[2], *permanentZones[10];
	UIElement* stackZone, * optionZone, * logbookZone;

	UIElement* new_final_element(int y, int z, int width, int height, int offset, int maxItems, bool direction);
public:
	explicit DefaultUI(ImplementIO* IOLib);
	~DefaultUI(){ delete playerPerms[0]; delete playerPerms[1]; delete optionZone; };
	UIElement* declare_element(int typeofelement, char owner_id);
	void get_player_coords(char player_id, Rect* zone, int* liby, int* libz, int* gravey, int* gravez, int* exily, int* exilz, int* cardzoneY, int* cardzoneZ);
	bool chooseattackers(PContainer<Creature>& cowards, PContainer<Creature>& warriors, char player_id);
	void chooseblockers(PContainer<Creature>& defenders, PContainer<Creature>& attackers, UIElement* defenderDisplay, UIElement* attackerDisplay);
	Creature* blocker_switch(const Creature& blocker, int blockerY, int blockerZ,
		PContainer<Creature>& attackers, UIElement* attacker_io) const ;
	void clear_opts();
	UIElement* get_optionzone();
	Target* iterate(bool needstarget, Player** pl, char returntypeflags);
	OptionAction* choose_opt(bool sorceryspeed, Player* asker);
	void normalize_gridy_gridz();
	void deadzone();
};

namespace AbstractUI {
    constexpr int ELTYPE_STACK = 0, ELTYPE_LOGBOOK = 1, ELTYPE_PERMANENTS = 2;
}

#endif //OLYMPUS_11_IMPLEMEMT_UI
