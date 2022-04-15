#ifndef OLYMPUS_CLASSES_OPTIONS_7_H
#define OLYMPUS_CLASSES_OPTIONS_7_H

#include "Mana/cost.h"
#include "1effects.h"
#include "displayable.h"
#include <memory>
class Resolvable; class Player; class Card; class Permanent;

/** Options are the standard format for any action the player can do.
* This standard only imposes that each Option has a castOpt method,
* which handles how the option behaves after having been chosen.
* Notably, Cards in a player's hand are Options. Technically, 'conceding' is an option. */
class Option : virtual public Displayable {
public:
    virtual ~Option() = default;
    virtual bool isCastable(bool sorceryspeed, const Player* player) const { (void) sorceryspeed; (void) player; return true; }
    // Whether it is possible to cast it on principle. Returns true by default.
    // Filters out more or less easy restrictions such as "only as a sorcery"
    virtual bool castOpt(Player* pl) = 0; //Actually try to cast the option. Returns false on failure.
};

class CostOption: public Option {
public:
    virtual bool isCastable(bool sorceryspeed, const Player* pl) const override;
    virtual Cost getCost() = 0;
};

class CardOption : public Option {
public:
    bool isCastable(bool sorceryspeed, const Player* player) const override;
    bool castOpt(Player* pl) override;
    std::string describe() const override;
    std::string describe(const std::string& cardName) const;
    void disp(Canvas* io, disp_flags flags) const override;
};

//There are multiple types of options:
// - casting a spell
// - activating an ability of a permanent or a card in {exile, graveyard, your hand, the command zone}
// - special actions: playing a land, morphing, "take an action at a later time", "take an action to ignore a rule", suspend, companions, foretell
// Special actions are so indistinguishable from normal abilities that I will ignore them for now.

#endif //OLYMPUS_CLASSES_OPTIONS_7_H
