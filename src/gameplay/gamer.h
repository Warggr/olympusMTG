#ifndef OLYMPUS_GAMER_H
#define OLYMPUS_GAMER_H

#include "1general.h"
#include "9modifs.h"
#include "3player_cards.h"
#include "yggdrasil/3player_board.h"
#include "Mana/lib2_mana.h"
#include <memory>

struct PlayerPreStackElement{
    Effect_H* preRes;
    std::unique_ptr<Targeter> origin;

    PlayerPreStackElement(Effect_H* p, Target* org): preRes(p) {
        origin = std::make_unique<Targeter>(org);
    };
};

struct Gamer: public Target, public Damageable {
public:
    using hand_type = std::list<CardWrapper>;
protected:
    std::string name;
    unsigned char
        phase : 3,
        milledout : 1,
        zerolife : 1,
        nb_mainphase : 3,
        nb_lands_remaining : 3
    ;
    static constexpr u_char 
        upkeep = 0b0,
        main = 0b001,
        afterattack = 0b010,
        afterblock = 0b011,
        afterdamage = 0b100,
        end = 0b110,
        nonactive = 0b111;
    std::forward_list<PlayerPreStackElement> prestack;
    CardZone myZones[4];
    hand_type myHand;
public:
    enum myzone { library, graveyard, exile, command };
#define myLibrary myZones[Gamer::myzone::library]
    Mana manapool;
    BoardN myboard;

    Gamer(): Target(name), Damageable(20), 
        phase(0), milledout(0), zerolife(0), nb_mainphase(0), nb_lands_remaining(1), 
        manapool(0) {}
    Gamer(std::string&& nm): Gamer() { name = nm; }
    void disp(Canvas* io, disp_flags flags) const override;
    std::string describe() const override;

    u_char getPhase() const { return phase; }
    hand_type& getHand() { return myHand; }
    const hand_type& getHand() const { return myHand; }
    Target* getMeAsTarget() override { return this; }
    Player* getDmgController() override;
    Player* getController() override;
    const CardZone& getZone(myzone zone) const { return myZones[zone]; }

    bool canPlayLand() const { return nb_lands_remaining != 0; }

    void addMana(char color);
    void emptyPool();

    void putToZone(card_ptr& x, myzone nb_zone);
};

#endif //OLYMPUS_GAMER_H
