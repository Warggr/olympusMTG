#ifndef OLYMPUS_CLASSES_OPTIONS_7_H
#define OLYMPUS_CLASSES_OPTIONS_7_H

#include "Mana/lib2_mana.h"
#include "1effects.h"
#include <memory>
class Resolvable; class Player; class Card; class Permanent; class WeirdCost;

//An option is a precise casting instruction.
//Together with an OptionWrapper, it represents an action you can do.
class Option {
protected:
    void payCosts(Player* pl) const;
public:
	Mana cost;
	WeirdCost* additional_costs = nullptr;
	bool instantspeed = false;
	Effect_H* effects = nullptr;
#ifdef F_TESTS
	bool exists;
	static char next_tag;
	char tag;

	Option(Option* pre, Option* nxt, Mana c): prev(pre), next(nxt), cost(c), instantspeed(0){exists = true; tag = next_tag; next_tag++;};
	virtual ~Option() {exists = false; };
#else
	Option() = default;
	explicit Option(Mana c, bool instantspeed = false): cost(c), instantspeed(instantspeed){};
	virtual ~Option() = default;
#endif
//	virtual void disp(int y, int z, int width, bool highlight, bool castable) const = 0;
    virtual bool iscastable(const Player* pl) const = 0;
};

class SpellOption: public Option {
    bool isLand;
public:
    explicit SpellOption(card_type type);
    explicit SpellOption(bool isLand = false, bool instantSpeed = false);
    virtual ~SpellOption() = default;
//    void disp(int y, int z, int width, bool highlight, bool castable) const override;
    virtual void cast_opt(Player* pl, uptr<Card>& origin);
    bool iscastable(const Player* pl) const override;
};

class CardOption: public SpellOption {
    bool isSpell, requiresDiscarding;
public:
    explicit CardOption(ReaderVisitor& visitor) { init(visitor); };
    void init(ReaderVisitor& visitor);
//	void disp(int y, int z, int width, bool highlight, bool castable) const override;
    std::string describe(const std::string& name) const;
};

class OptionAction {
public:
    virtual ~OptionAction() = default;
    virtual void cast_opt(Player* pl) = 0;
};

class SpellOptionAction : public OptionAction {
    SpellOption& option;
    uptr<Card>& optionHolder;
public:
    SpellOptionAction(SpellOption& option, uptr<Card>& holder): option(option), optionHolder(holder) {};
    void cast_opt(Player* pl) override {
        option.cast_opt(pl, optionHolder);
    }
};

#endif //OLYMPUS_CLASSES_OPTIONS_7_H
