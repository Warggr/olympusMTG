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

enum cardzones { library_zn, graveyard_zn, exile_zn };

class Player: public Target, public Damageable {
private:
	std::string name;
	unsigned char phase : 3, hasplayedland : 1, milledout : 1, zerolife : 1;
	//upkeep (000) main1(001) afterattack(010) afterblock(011) afterfirstdamage(100) main2(101) end(110) nonactive(111)
	CardZone myzones[3]; //library - graveyard - exile
	std::forward_list<PlayerPreStackElement> prestack;
	Mana manapool;
	char nb_mainphase;
	Player* nextopponent;
    TriggerEvent triggers[2]; //gain/lose life - is damaged -
public:
	static const std::forward_list<const Phase*> defaultPhaseOrder; //this will be copied and used as a template at the beginning of each turn

	Agent& agent;
	BoardN myboard;

	Player(Agent& agent, CardZone library);
	~Player();

	inline void set_opp(Player* opp){ nextopponent = opp; };
	inline bool hasPlayedLand() const { return hasplayedland; };
    void damage(int nb_damage, Target* origin) override;
    inline void gain_life(int nb_life) { life += nb_life; };
	void disp() const;
	void disp_header(bool highlight = false) const;
	void disp_zone(int nbzone) const;
/*	Player* iterate_self(DirectioL& direction);
	template <class PermType>
		PermType* iterate_boardsubzone(float offset, DirectioL& direction, PContainer<PermType>* perms, UIElement* ui, bool isactivation);
	Permanent* iterate_boardsubzone(float offset, DirectioL& direction, int xzone, bool istapland);*/

	inline void addtoPrestack(Effect_H* triggered_ability, Target* origin){ prestack.emplace_front(triggered_ability, origin); };
	bool add_triggers_to_stack();

	bool choose_and_use_opt(bool sorceryspeed);
	void choicephase(bool sorceryspeed);
	
	void insert_permanent(uptr<Card>& source);
	void remove_permanent(Permanent* perm, int nb_zone);
//	template <class PermType> void remove_permanent_inlist(PContainer<PermType>* perms, PermType* asup);
	void resolve_playland(Card* source);
	bool chooseattackers();
//	void chooseblockers(StatedCollectionTN<Creature>& attackers, UIElement* attackerDisplay);

	void draw(int nb_cards);
	void add_mana(char color);
	void puttozone(std::unique_ptr<Card>& x, char n);

	void check_too_expensive();
	void empty_pool();

	bool statebasedactions();
	bool turn();

	Identifier reload_id() const;
	Player* getController() override { return this; }
};

#endif //OLYMPUS_CLASSES_PLAYER_3_H