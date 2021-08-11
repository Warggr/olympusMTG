#ifndef OLYMPUS_CLASSES_PERMANENTS_4_H
#define OLYMPUS_CLASSES_PERMANENTS_4_H


#include "gameplay/1general.h"
#include "oracles/classes/1effects.h"
#include "oracles/classes/3statics.h"
#include "headI_identifiers.h"
#include <memory>
#include <list>
#include <agents/agent.h>

class PermOption;

#define uptr std::unique_ptr

class Permanent: public Target, public OptionWrapper<PermOption> {
protected:
	TriggerEvent triggers_permanent[4]; //becomes (state) - becomes (special) - ETB - LTB
	std::unique_ptr<Card> source;
	Player* ctrl;
	PermOption* first_actab;
	ModifListNode* existing_statics;
	int nb_actabs;
	char etbBeforeThisTurn : 1, untapped : 1;
	char keywords; //indestructible -
	char color;
public:
    typedef permanent_type type;

	Permanent(uptr<Card> source, Player* pl);
	virtual ~Permanent() = default;
	virtual std::string describe() const;
//	virtual void disp(const Rect& zone, bool highlight = false) const;

    virtual void tap() { untapped = 0; }
	virtual void untap() { untapped = 1; }
	virtual void declare_beginturn(){ untap(); etbBeforeThisTurn = 1; };
	virtual void activate();
//	bool directactivate();
	virtual void destroy();
	virtual void exile();
    type getType() const;

    Player* getController() override { return ctrl; }
};

class Artifact: public Permanent{
public:
	static constexpr Identifier def_identifier = construct_id_perm(permanent_type::artifact, 0, 0);

	Artifact(uptr<Card> src, Player* pl): Permanent(std::move(src), pl){};
//	Identifier reload_id() const override;
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
	std::list<std::pair<uint8_t, SpecificTargeter<Creature>>> assigned_bl; //might become invalidated if the attacker dies midcombat

public:
	static constexpr Identifier def_identifier = construct_id_perm(permanent_type::creature, 0, false);

	Creature(std::unique_ptr<Card> src, Player* pl);
	std::string describe() const override;

//	void disp(const Rect& zone, bool highlight) const;

//	void damage(int nb_damage, Target* origin = 0) override;
	void hit(Damageable* tgt);
	void add_blocker(Creature* bl);
	void set_blocking(){is_block = true; };
	void resolve_attack(Player* nextopponent);
	int get_power() const {if(pt_switched) return set_power + added_power; else return set_toughness + added_toughness; };
	int get_toughness() const {if(pt_switched) return set_toughness + added_toughness; else return set_power + added_power; };
	void plus_power(char dpower){added_power += dpower; };
	void plus_toughness(char dtoughness){added_toughness += dtoughness; };

//    Identifier reload_id() const override;
    Player* getDmgController() override { return ctrl; }

    void splitDamage(Agent &agent);
};

class Planeswalker: public Permanent, public Damageable{
    /*Effect_H* loyalty_abs;
    char* loyalty_costs;
    int loyalty; */
    bool thisturn; //TODO planeswalker abilities
public:
//	static constexpr Identifier def_identifier = construct_id_perm(planeswalker, 0, false);

	Planeswalker(uptr<Card> src, Player* pl);
//	void damage(int nb_damage, Target* origin) override;
	void activate() override;
	void declare_beginturn() override { Permanent::declare_beginturn(); thisturn = false; };

//	Identifier reload_id() const override;
    Player* getDmgController() override { return ctrl; }
};

class Land: public Permanent{
public:
	static constexpr Identifier def_identifier = construct_id_perm(permanent_type::land, 0, 0);

	Land(uptr<Card> src, Player* pl);

//	void disp(const Rect& zone, bool highlight) const;
	void untap() override;

//	Identifier reload_id() const override;
};

#endif //OLYMPUS_CLASSES_PERMANENTS_4_H
