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
	float mousey{0};
	float mousez{0};
	_UIElement* playerPerms[2], *permanentZones[10];
	_UIElement* stackZone, * optionZone, * logbookZone;
public:
	Default_ui(Abstract_io* IOLib);
	~Default_ui(){delete playerPerms[0]; delete playerPerms[1]; delete optionZone; };
	UIElement* declare_element(int typeofelement, char owner_id);
	void get_player_coords(char player_id, int* py, int* pz, int* wide, int* high, int* liby, int* libz, int* gravey, int* gravez, int* exily, int* exilz);
	bool chooseattackers(std::list<Creature>& cowards, std::list<Creature>& warriors, char player_id);
	void chooseblockers(std::list<Creature>& defenders, std::list<Creature>& attackers, UIElement* defenderDisplay, UIElement* attackerDisplay);
	void clear_opts();
	UIElement* get_optionzone();
	Target* iterate(bool needstarget, Player** pl, char returntypeflags);
	Option* choose_opt(float zOffset, bool sorceryspeed, Option* firstopt, Player* asker, int metapos);
	void report_mouse_move(float y, float z){mousey = y; mousez = z; };
	void normalize_gridy_gridz(int* gridy, int* gridz);
};

#endif //OLYMPUS_11_IMPLEMENT_UI