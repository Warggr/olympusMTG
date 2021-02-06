#ifndef OLYMPUS_11_IMPLEMEMT_UI
#define OLYMPUS_11_IMPLEMENT_UI
#define MANUAL_INCLUDE_OF_OLYMPUS_HEADERS
#include "../include/.olympus_main.h"
#include "../include/6abstractIO.h"

class Default_ui: public Abstract_ui{
private:
	static const int spaceY, spaceZ;
	static const int leftbarY, boardY, rightbarY;
	static const int stackZ, optionsZ, playerZ, iozZ;
	int posterZ;
	static const int permanentZSize, permanentYSize;
	static const int optionZSize;
	static const int permanentZMargin;
	int gridy{0}, gridz{0};
	DirectioL direction;
	_UIElement* playerPerms[2], *permanentZones[10];
	_UIElement* stackZone, * optionZone, * logbookZone;
public:
	Default_ui(Abstract_io* IOLib);
	~Default_ui(){delete playerPerms[0]; delete playerPerms[1]; delete optionZone; };
	UIElement* declare_element(int typeofelement, char owner_id);
	void get_player_coords(char player_id, int* py, int* pz, int* wide, int* high, int* liby, int* libz, int* gravey, int* gravez, int* exily, int* exilz);
	bool chooseattackers(PContainer<Creature>& cowards, PContainer<Creature>& warriors, char player_id);
	void chooseblockers(PContainer<Creature>& defenders, PContainer<Creature>& attackers, UIElement* defenderDisplay, UIElement* attackerDisplay);
	void clear_opts();
	UIElement* get_optionzone();
	Target* iterate(bool needstarget, Player** pl, char returntypeflags);
	Option* choose_opt(bool sorceryspeed, Option* firstopt, Player* asker, int metapos);
	void normalize_gridy_gridz();
	void deadzone();
};

#endif //OLYMPUS_11_IMPLEMENT_UI