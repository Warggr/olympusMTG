#ifndef OLYMPUS_PLAYER_H
#define OLYMPUS_PLAYER_H

#include "classes/1effects.h"
#include "gameplay/1general.h"
#include "gameplay/9modifs.h"
#include "Mana/lib2_mana.h"
#include "gameplay/3player_cards.h"
#include "yggdrasil/3player_board.h"
#include "optionmanager.h"
#include "agents/agent.h"
#include <memory>

struct Phase;

struct PlayerPreStackElement{
    Effect_H* preRes;
    std::unique_ptr<Targeter> origin;

    PlayerPreStackElement(Effect_H* p, Target* org): preRes(p) {
        origin = std::make_unique<Targeter>(org);
    };
};

class Player: public Target, public Damageable {
public:
    using hand_type = std::list<CardWrapper>;
private:
    Agent& agent;
    Viewer& viewer;
    std::string name;
    unsigned char phase : 3, milledout : 1, zerolife : 1, nb_mainphase : 3, nb_lands_remaining : 3;
    static constexpr u_char upkeep = 0b0, main = 0b001, afterattack = 0b010, afterblock = 0b011,
        afterdamage = 0b100, end = 0b110, nonactive = 0b111;
    CardZone myZones[4];
    std::forward_list<PlayerPreStackElement> prestack;
    hand_type myHand;
    Player* nextopponent;
    TriggerEvent triggers[1]; //gain/lose life
public:
    enum myzone { library, graveyard, exile, command };
#define myLibrary myZones[Player::myzone::library]
    static const std::forward_list<const Phase*> defaultPhaseOrder;
    //this will be copied and used as a template at the beginning of each turn

    BoardN myboard;
    OptionManager myOptions;
    Mana manapool;

    Player(Agent& agent, CardZone&& library);

    inline bool canPlayLand() const { return nb_lands_remaining != 0; };
    inline void gainLife(int nb_life) { life += nb_life; triggers[0].trigger(this, nullptr); }

    inline void addToPrestack(Effect_H* triggered_ability, Target* origin){ prestack.emplace_front(triggered_ability, origin); }
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

    Player* getDmgController() override { return this; }
    Target* getMeAsTarget() override { return this; }
    Player* getController() override { return this; }
    Agent& getAgent() const { return agent; }
    u_char getPhase() const { return phase; }

    void disp(BasicIO* io) const override;
    std::string describe() const override;

    hand_type& getHand() { return myHand; }
    const hand_type& getHand() const { return myHand; }
    const CardZone& getZone(myzone zone) const { return myZones[zone]; }

    friend class OptionManager;
};

#endif //OLYMPUS_PLAYER_H