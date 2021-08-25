#ifndef OLYMPUS_11_IMPLEMENT_UI
#define OLYMPUS_11_IMPLEMENT_UI

#include "build_types.h"
#include "../io/6abstractIO.h"
#include "window.h"
class Permanent; class Creature; class Player; class Target; class OptionAction;
template<typename T> class CollectionTN; template<typename T> class StateTN;

class DefaultUI {
private:
    AbstractIO* myIO;
	int leftbarW, boardW, rightbarW;
	int stackH, optionsH, playerH, iozH, posterH;
	int permanentZSize, permanentYSize, permanentZMargin;
	int optionZSize;
	int playerY[2], playerZ[2];
	int gridy{0}, gridz{0};
	int linesize;
    bool mouseSupport;

    ModernElement screen;
	DirectioL direction;
	ListElement* permanentZones[2];
	ListElement* optionZone, * logbookZone;

//	Window* new_final_element(int y, int z, int width, int height, int offset, int nbItems, bool direction);
public:
	explicit DefaultUI(AbstractIO* IOLib);
	~DefaultUI(){ delete optionZone; };
	void get_player_coords(char player_id, Rect* zone, int* liby, int* libz, int* gravey, int* gravez, int* exily, int* exilz, int* cardzoneY, int* cardzoneZ);
	bool chooseattackers(CollectionTN<Creature>& cowards, StateTN<Creature>& warriors);
	void chooseblockers(CollectionTN<Creature>& defenders, StateTN<Creature>& attackers);
	Creature* blocker_switch(const Creature& blocker, int blockerY, int blockerZ,
		StateTN<Creature>& attackers) const ;
	void clear_opts();
	Target* iterate(bool needstarget, char returntypeflags);
	uptr<OptionAction> chooseOpt(bool sorceryspeed, Player* asker);
	void normalize_gridy_gridz();
	void deadzone();
};

namespace AbstractUI {
    constexpr int ELTYPE_STACK = 0, ELTYPE_LOGBOOK = 1, ELTYPE_PERMANENTS = 2;
}

#endif //OLYMPUS_11_IMPLEMENT_UI
