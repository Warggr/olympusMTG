#ifndef OLYMPUS_PERMANENTS_4_H
#define OLYMPUS_PERMANENTS_4_H

#include "../1general.h"
#include "../optionwrappers.h"
#include "oracles/classes/1effects.h"
#include "oracles/classes/3statics.h"
#include "headI_identifiers.h"
#include "headE_enums.h"
#include "Mana/lib2_mana.h"
#include <memory>
#include <list>
#include <agents/agent.h>

class PermOption;

class Permanent: public Target, public SpecificOptionWrapper<PermOption> {
protected:
	TriggerEvent triggers_permanent[4]; //becomes (state) - becomes (special) - ETB - LTB
	std::unique_ptr<Card> source;
	Player* ctrl;
	PermOption* first_actab;
	int nb_actabs;
	uchar etbBeforeThisTurn : 1, untapped : 1;
	char keywords; //indestructible -
	colorId::type color;
public:
    typedef permanent_type type;

	Permanent(uptr<Card> source, Player* pl);
	virtual ~Permanent() = default;
	virtual std::string describe() const;

    bool isUntapped() const { return untapped; }
    bool noSummonSick() const { return etbBeforeThisTurn; }
    virtual void tap() { untapped = 0; }
	virtual void untap() { untapped = 1; }
	virtual void declareBeginturn(){ untap(); etbBeforeThisTurn = 1; };
	virtual void activate();
//	bool directactivate();
	virtual void destroy();
	virtual void exile();
    type getType() const;
    Mana getManaCost() const;

    Player* getController() override { return ctrl; }
    uptr<OptionAction> chooseOptionAction() override;

    friend class AbstractIO;
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

//	void damage(int nb_damage, Target* origin = 0) override;
	void hit(Damageable* tgt);
	void addBlocker(Creature* bl);
	void setBlocking(){is_block = true; };
	void resolveAttack(Player* nextopponent);
	int getPower() const {if(pt_switched) return set_power + added_power; else return set_toughness + added_toughness; };
	int getToughness() const {if(pt_switched) return set_toughness + added_toughness; else return set_power + added_power; };
	void plusPower(char dpower){added_power += dpower; };
	void plusToughness(char dtoughness){added_toughness += dtoughness; };

    Player* getDmgController() override { return ctrl; }

    void splitDamage(Agent &agent);

    friend class AbstractIO;
};

class Planeswalker: public Permanent, public Damageable{
    /*Effect_H* loyalty_abs;
    char* loyalty_costs;
    int loyalty; */
    bool thisturn; //TODO planeswalker abilities
public:

	Planeswalker(uptr<Card> src, Player* pl);
//	void damage(int nb_damage, Target* origin) override;
	void activate() override;
	void declareBeginturn() override { Permanent::declareBeginturn(); thisturn = false; };

    Player* getDmgController() override { return ctrl; }

    friend class AbstractIO;
};

#endif //OLYMPUS_PERMANENTS_4_H