#ifndef OLYMPUS_CLASSES_OPTIONS_7_H
#define OLYMPUS_CLASSES_OPTIONS_7_H

#include "Mana/lib2_mana.h"
#include "1effects.h"
#include "displayable.h"
#include <memory>
class Resolvable; class Player; class Card; class Permanent; class WeirdCost;

//OptionAction are the standard format for any action the player can do.
class Option : public Displayable {
public:
    virtual ~Option() = default;
    virtual bool isCastable(bool sorceryspeed) const = 0;
    // Whether it is possible to cast it on principle. Returns true by default.
    // Filters out more or less easy restrictions such as "only as a sorcery"
    virtual void castOpt(Player* pl) = 0;
};

//There are multiple types of options:
// - casting a spell
// - activating an ability of a permanent or a card in {exile, graveyard, your hand, the command zone}
// - special actions: playing a land, morphing, "take an action at a later time", "take an action to ignore a rule", suspend, companions, foretell
// Special actions are so indistinguishable from normal abilities that I will ignore them for now.

class DefaultCardOption: public Option {
protected:
    uptr<Card> origin;
public:
    bool isCastable(bool sorceryspeed) const override;
    void castOpt(Player *pl) override;
};

class CardOption: public Option {
public:
    bool isCastable(bool sorceryspeed) const override;
    void castOpt(Player *pl) override;
    std::string describe() const override;
    std::string describe(const std::string& cardName) const;
};

#endif //OLYMPUS_CLASSES_OPTIONS_7_H
