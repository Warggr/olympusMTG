#ifndef OLYMPUS_PLAYER_H
#define OLYMPUS_PLAYER_H

#include "optionmanager.h"
#include "gameplay/gamer.h"
#include "classes/1effects.h"
#include "Mana/lib2_mana.h"
#include <memory>

struct Phase; class Agent; class Viewer;

class Player: public Gamer {
private:
    Agent& agent;
    Viewer& viewer;
    std::forward_list<PlayerPreStackElement> prestack;
    Player* nextopponent;
    TriggerEvent triggers[1]; //gain/lose life
public:
    static const std::forward_list<const Phase*> defaultPhaseOrder;
    //this will be copied and used as a template at the beginning of each turn

    OptionManager myOptions;

    Player(Agent& agent, CardZone&& library);

    bool canPlayLand() const { return nb_lands_remaining != 0; };
    void gainLife(int nb_life) { life += nb_life; triggers[0].trigger(this, nullptr); }

    void addToPrestack(Effect_H* triggered_ability, Target* origin){ prestack.emplace_front(triggered_ability, origin); }
    bool addTriggersToStack();

    bool chooseAndUseOpt(bool sorceryspeed);
    void choicephase(bool sorceryspeed);

    void resolvePlayland(uptr<Card> source);
    void chooseBlockers(StateTN<Creature>& attackers);

    void draw(int nb_cards);
    void drawStartingHand();
    void addMana(char color);
    void putToZone(std::unique_ptr<Card>& x, myzone nb_zone);

    bool pay(Cost cost);
    void emptyPool();

    bool stateBasedActions();
    bool turn();

    void untapstep(), upkeepstep(), drawstep(),
    mainphasemainstep(), declareattackersstep(), declareblockersstep(), damagestep(),
    endstep(), cleanupstep();

    inline void setOpp(Player* opp){ nextopponent = opp; };

    Agent& getAgent() const { return agent; }

    friend class OptionManager;
};

#endif //OLYMPUS_PLAYER_H
