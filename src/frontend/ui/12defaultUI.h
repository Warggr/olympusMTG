#ifndef OLYMPUS_11_IMPLEMEMT_UI
#define OLYMPUS_11_IMPLEMENT_UI

#include ".build_types.h"
#include "../io/6abstractIO.h"

class Default_ui: public Abstract_ui {
private:
	int leftbarW, boardW, rightbarW;
	int stackH, optionsH, playerH, iozH, posterH;
	int permanentZSize, permanentYSize, permanentZMargin;
	int optionZSize;
	int playerY[2], playerZ[2];
	int gridy{0}, gridz{0};
	int linesize;

	DirectioL direction;
	_UIElement* playerPerms[2], *permanentZones[10];
	_UIElement* stackZone, * optionZone, * logbookZone;

	_UIElement* new_final_element(int y, int z, int width, int height, int offset, int maxItems, bool direction);
public:
	Default_ui(Abstract_io* IOLib);
	~Default_ui(){delete playerPerms[0]; delete playerPerms[1]; delete optionZone; };
	UIElement* declare_element(int typeofelement, char owner_id);
	void get_player_coords(char player_id, Rect* zone, int* liby, int* libz, int* gravey, int* gravez, int* exily, int* exilz, int* cardzoneY, int* cardzoneZ);
	bool chooseattackers(PContainer<Creature>& cowards, PContainer<Creature>& warriors, char player_id);
	void chooseblockers(PContainer<Creature>& defenders, PContainer<Creature>& attackers, UIElement* defenderDisplay, UIElement* attackerDisplay);
	Creature* blocker_switch(const Creature& blocker, int blockerY, int blockerZ,
		PContainer<Creature>& attackers, UIElement* attacker_io) const ;
	void clear_opts();
	UIElement* get_optionzone();
	Target* iterate(bool needstarget, Player** pl, char returntypeflags);
	Option* choose_opt(bool sorceryspeed, Player* asker);
	void normalize_gridy_gridz();
	void deadzone();
};

#endif //OLYMPUS_11_IMPLEMEMT_UI