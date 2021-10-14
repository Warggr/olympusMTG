#ifndef OLYMPUS_11_IMPLEMENT_UI
#define OLYMPUS_11_IMPLEMENT_UI

#include "build_types.h"
#include "../io/6abstractIO.h"
#include "window.h"
#include "argus.h"

class Permanent; class Creature; class Player; class Target; class OptionAction;
template<typename T> class Y_Hashtable; template<typename T> class StateTN;

class DefaultUI {
private:
    AbstractIO* myIO;
	int playerY[2], playerZ[2];
	int linesize;
    bool mouseSupport;

    Screen screen;
	DirectioL direction;

public:
	explicit DefaultUI(AbstractIO* IOLib);
	void registerPlayers(std::list<Player>& players);

	bool chooseattackers(Y_Hashtable<Creature>& cowards);
	void chooseblockers(Y_Hashtable<Creature>& defenders, StateTN<Creature>& attackers);
	Creature* blockerSwitch(const Creature& blocker, int blockerIndex, StateTN<Creature>& attackers);

	Target* iterate(bool needstarget, char returntypeflags);
	uptr<OptionAction> chooseOpt(bool sorceryspeed);

	void addCards(const std::list<uptr<Card>>& cards);

	void fullDisp();
    void clear_opts();
};

namespace AbstractUI {
    constexpr int ELTYPE_STACK = 0, ELTYPE_LOGBOOK = 1, ELTYPE_PERMANENTS = 2;
}

#endif //OLYMPUS_11_IMPLEMENT_UI
