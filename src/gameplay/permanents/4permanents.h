#ifndef OLYMPUS_CLASSES_PERMANENTS_4_H
#define OLYMPUS_CLASSES_PERMANENTS_4_H


#include "gameplay/1general.h"
#include "oracles/classes/1effects.h"
#include "oracles/classes/3statics.h"
#include "oracles/classes/4actabs.h"
#include "headI_identifiers.h"
#include <memory>
#include <list>
class PermOption;

class Permanent: public Target, public OptionWrapper<PermOption> {
protected:
	TriggerEvent triggers_permanent[4]; //becomes (state) - becomes (special) - ETB - LTB
	std::unique_ptr<Card> source;
	Player* ctrl;
	ActAb_H* first_actab;
	ModifListNode* existing_statics;
	int nb_actabs;
	char etbBeforeThisTurn : 1, untapped : 1;
	char keywords; //indestructible -
	char color;
public:
	Permanent(std::unique_ptr<Card> source, Player* pl, int nb_zone);
	virtual ~Permanent() = default;
	virtual std::string describe() const;
	virtual void disp(const Rect& zone, bool highlight = false) const;

	virtual void untap(){ untapped = 1; }
	virtual void declare_beginturn(){ untap(); etbBeforeThisTurn = 1; };
	virtual void activate();
	bool directactivate();
	virtual void destroy();
	virtual void exile();
	//void merge_source(Card* src_copy){delete source; source = src_copy; };
};

class Artifact: public Permanent{
public:
	static constexpr Identifier def_identifier = construct_id_perm(artifact, 0, 0);

	Artifact(std::unique_ptr<Card> src, Player* pl): Permanent(std::move(src), pl, 1){};
	Identifier reload_id() const override;
};

class Creature: public Permanent, public Damageable{
private:
	TriggerEvent triggers_creature[4]; //attacks - blocks - becomes blocked - isn't blocked
	int self_power, self_toughness; //layer 7a, characteristics-defining effects
	int set_power, set_toughness; //layer 7b
	int added_power{0}, added_toughness{0}; //layer 7c. By default these values are volatile and are reset at end of turn
	int nb_counters{0};
	bool pt_switched{false}; //layer 7d

	bool is_attacking{false}, is_block{false}; //used for both blocking and blocked
	std::list<Targeter*> assigned_bl; //might become invalidated if the attacker dies midcombat

public:
	static constexpr Identifier def_identifier = construct_id_perm(creature, 0, 0);

	Creature(std::unique_ptr<Card> src, Player* pl);
	std::string describe() const;

	void disp(const Rect& zone, bool highlight) const;

	void damage(int nb_damage, Target* origin = 0) override;
	void hit(Damageable* tgt) const;
	void add_blocker(Creature* bl);
	void set_blocking(){is_block = true; };
	void resolve_attack(Player* nextopponent);
	int get_power() const {if(pt_switched) return set_power + added_power; else return set_toughness + added_toughness; };
	int get_toughness() const {if(pt_switched) return set_toughness + added_toughness; else return set_power + added_power; };
	void plus_power(char dpower){added_power += dpower; };
	void plus_toughness(char dtoughness){added_toughness += dtoughness; };

    Identifier reload_id() const override;
    Player* getController() override { return ctrl; }
};

class Planeswalker: public Permanent, public Damageable{
    Effect_H* loyalty_abs;
    char* loyalty_costs;
    int loyalty;
    bool thisturn;
public:
	static constexpr Identifier def_identifier = construct_id_perm(planeswalker, 0, false);

	Planeswalker(std::unique_ptr<Card> src, Player* pl);
	char target_type() const{return 0x48; }; //Permanent and Planeswalker
	void damage(int nb_damage, Target* origin) override;
	void activate() override;
	void declare_beginturn() override { Permanent::declare_beginturn(); thisturn = false; };

	Identifier reload_id() const override;
    Player* getController() override { return ctrl; }
};

class Land: public Permanent{
public:
	static constexpr Identifier def_identifier = construct_id_perm(land, 0, 0);

	Land(Card* src, Player* pl);

	void disp(const Rect& zone, bool highlight) const;
	void untap() override;

	Identifier reload_id() const override;
};

#endif //OLYMPUS_CLASSES_PERMANENTS_4_H
