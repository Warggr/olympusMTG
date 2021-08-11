#ifndef OLYMPUS_CLASSES_OPTIONS_7_H
#define OLYMPUS_CLASSES_OPTIONS_7_H

#include "Mana/lib2_mana.h"
#include "1effects.h"
#include <memory>
class Resolvable; class Player; class Card; class Permanent; class WeirdCost;
#define uptr std::unique_ptr

//An option is a precise casting instruction.
//Together with an OptionWrapper, it represents an action you can do.
template<typename T>
class Option {
protected:
    void payCosts(Player* pl);
public:
	Mana cost;
	WeirdCost* additional_costs;
	bool instantspeed;
	Effect_H* effects;
#ifdef F_TESTS
	bool exists;
	static char next_tag;
	char tag;

	Option(Option* pre, Option* nxt, Mana c): prev(pre), next(nxt), cost(c), instantspeed(0){exists = true; tag = next_tag; next_tag++;};
	virtual ~Option() {exists = false; };
#else
	Option() = default;
	explicit Option(Mana c, bool instantspeed = false): cost(c), instantspeed(instantspeed){};
	virtual ~Option();
#endif
	virtual void cast_opt(Player* pl, T& origin) = 0;
//	virtual void disp(int y, int z, int width, bool highlight, bool castable) const = 0;
    virtual bool iscastable(const Player* pl) const = 0;
};

class SpellOption: public Option<uptr<Card>> {
    bool isLand;
public:
    explicit SpellOption(card_type type);
    explicit SpellOption(bool isLand = false, bool instantSpeed = false);
    virtual ~SpellOption();
//    void disp(int y, int z, int width, bool highlight, bool castable) const override;
    virtual void cast_opt(Player* pl, uptr<Card>& origin) override;
    bool iscastable(const Player* pl) const override;
};

class CardOption: public SpellOption {
    bool isSpell, requiresDiscarding;
public:
    explicit CardOption(ReaderVisitor& visitor) { init(visitor); };
    void init(ReaderVisitor& visitor);
//	void disp(int y, int z, int width, bool highlight, bool castable) const override;
};

class OptionAction {
public:
    virtual void cast_opt(Player* pl) = 0;
};

template<typename T>
class OptionActionTpl : public OptionAction {
    Option<T>& option;
    T& optionHolder;

    void cast_opt(Player* pl) override {
        option.cast_opt(pl, optionHolder);
    }
};

#endif //OLYMPUS_CLASSES_OPTIONS_7_H
