#ifndef OLYMPUS_CLASSES_PLAYER_3_H
#define OLYMPUS_CLASSES_PLAYER_3_H

#include <memory>
#include <thread>

#include "1general.h"
#include "Mana/lib2_mana.h"
#include "../Yggdrasil/headY_yggdrasil.h" //needs to know what a zone is

struct PlayerPreStackElement{
	PreResolvable* preRes;
	std::unique_ptr<Targeter> origin;

	PlayerPreStackElement(PreResolvable* p, Target* org);
};

class Player: public Target, public Damageable{
private: //TODO optimize by taking alignment into account (i.e. put chars after each other)
	std::string name;
	unsigned char phase : 3, hasplayedland : 1, milledout : 1, zerolife : 1;
	//upkeep (000) main1(001) afterattack(010) afterblock(011) afterfirstdamage(100) main2(101) end(110) nonactive(111)
	CardZone myzones[3]; //library - graveyard - exile
	std::forward_list<PlayerPreStackElement> prestack;
	Mana manapool;
	char nb_mainphase;
public:
	static const std::forward_list<const Phase*> defaultPhaseOrder; //this will be copied and used as a template at the beginning of each turn

	Agent& agent;
	BoardN myboard;

	Player(Agent& agent, std::forward_list<Card*> library);
	~Player();

	inline void set_opp(Player* opp){ nextopponent = opp; };
	inline bool hasplayedland() const { return hasplayedland; };
	void damage(int nb_damage, Target* origin);

	void disp() const;
	void disp_header(bool highlight = false) const;
	void disp_zone(int nbzone) const;
	Player* iterate_self(DirectioL& direction);
	template <class PermType>
		PermType* iterate_boardsubzone(float offset, DirectioL& direction, PContainer<PermType>* perms, UIElement* ui, bool isactivation);
	Permanent* iterate_boardsubzone(float offset, DirectioL& direction, int xzone, bool istapland);

	inline void addtoPrestack(PreResolvable* triggered_ability, Target* origin){ prestack.emplace_front(triggered_ability, origin); };
	bool add_triggers_to_stack();

	bool choose_and_use_opt(bool sorceryspeed);
	void choicephase(bool sorceryspeed);
	
	void insert_permanent(Card* source);
	void remove_permanent(Permanent* perm, int nb_zone);
	template <class PermType> void remove_permanent_inlist(PContainer<PermType>* perms, PermType* asup);
	void resolve_playland(Card* source);
	bool chooseattackers();
	void chooseblockers(StatedCollectionTN<Creature>& attackers, UIElement* attackerDisplay);

	void draw(int nb_cards);
	inline void gain_life(int nb_life) {life += nb_life; };
	void add_mana(char color);
	void puttozone(Card* x, char n);

	void check_too_expensive();
	void empty_pool();

	bool statebasedactions();
	bool turn();

	Identifier reload_id() const;

	void waitForSetup();
};

#endif //OLYMPUS_CLASSES_GAME_1_H