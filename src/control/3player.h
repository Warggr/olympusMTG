#ifndef OLYMPUS_CLASSES_PLAYER_3_H
#define OLYMPUS_CLASSES_PLAYER_3_H

#include "oracles/classes/1effects.h"
#include "gameplay/1general.h"
#include "gameplay/9modifs.h"
#include "Mana/lib2_mana.h"
#include "gameplay/3player_cards.h"
#include "yggdrasil/3player_board.h"
#include "agents/agent.h"
#include <memory>
#include <thread>

#define uptr std::unique_ptr

struct Phase;

struct PlayerPreStackElement{
	Effect_H* preRes;
	std::unique_ptr<Targeter> origin;

	PlayerPreStackElement(Effect_H* p, Target* org): preRes(p) {
	    origin = std::make_unique<Targeter>(org);
	};
};

enum cardzone { library_zn, graveyard_zn, exile_zn };

class Player: public Target, public Damageable {
private:
    Agent& agent;
	std::string name;
	unsigned char phase : 3, milledout : 1, zerolife : 1, nb_mainphase : 3, nb_lands_remaining : 3;
	static constexpr u_char upkeep = 0b0, main = 0b001, afterattack = 0b010, afterblock = 0b011,
	    afterdamage = 0b100, end = 0b110, nonactive = 0b111;
	CardZone myLibrary, myGraveyard, myExile; //myLibrary - myGraveyard - myExile
	std::forward_list<PlayerPreStackElement> prestack;
	std::list<uptr<Card>> myHand;
	Player* nextopponent;
    TriggerEvent triggers[1]; //gain/lose life
public:
	static const std::forward_list<const Phase*> defaultPhaseOrder;
	//this will be copied and used as a template at the beginning of each turn

	BoardN myboard;
    Mana manapool;

	Player(Agent& agent, CardZone&& library);

	inline void set_opp(Player* opp){ nextopponent = opp; };
	inline bool canPlayLand() const { return nb_lands_remaining != 0; };
    inline void gain_life(int nb_life) { life += nb_life; triggers[1].trigger(this, nullptr); }

	inline void addtoPrestack(Effect_H* triggered_ability, Target* origin){ prestack.emplace_front(triggered_ability, origin); }
	bool add_triggers_to_stack();

	bool choose_and_use_opt(bool sorceryspeed);
	void choicephase(bool sorceryspeed);
	bool hasOptions(bool sorcerySpeed) const;

	void insert_permanent(uptr<Card> source);
//	void remove_permanent(Permanent* perm, permanent_type type);
	template<typename permType> void remove_permanent(permType* perm) {
	    myboard.remove(perm);
	}
//	template <class PermType> void remove_permanent_inlist(PContainer<PermType>* perms, PermType* asup);
	void resolve_playland(uptr<Card> source);
	bool chooseAttackers();
	void chooseBlockers(StateTN<Creature>& attackers);

	void draw(int nb_cards);
	void add_mana(char color);
	void puttozone(std::unique_ptr<Card>& x, enum cardzone zone);

	void empty_pool();

	bool statebasedactions();
	bool turn();

//	Identifier reload_id() const;

	void untapstep(), upkeepstep(), drawstep(),
	mainphasemainstep(), declareattackersstep(), declareblockersstep(), damagestep(),
	endstep(), cleanupstep();

	Player* getDmgController() override { return this; }
    Player* getController() override { return this; }
    Agent& getAgent() const { return agent; }

    void drawStartingHand();
};

#endif //OLYMPUS_CLASSES_PLAYER_3_H