#include "frontend/basicIO.h"
#include "gameplay/permanents/4permanents.h"
#include "gameplay/2cards.h"
#include "gameplay/resolvables/5resolvables.h"
#include "control/3player.h"
#include "classes/perm_option.h"

void Permanent::disp(BasicIO* io) const {
    io->disp(*source->oracle, 0);
}

void Spell::disp(BasicIO *io) const {
    io->disp(*source->oracle, 0);
}

void Card::disp(BasicIO *io) const {
    io->disp(*oracle, 0);
}

void Player::disp(BasicIO *io) const {
    io->disp_player(*this, 0);
}

void Resolvable::disp(BasicIO* io) const {
    (void) io; //TODO
}

void PermOption::disp(BasicIO *io) const {
    (void) io; //TODO
}

void BasicIO::chooseAttackers(StateTN<Creature>& attackers) {
    StateTN<Creature> stay = attackers.anti();

    std::vector<bool> selected(stay.size(), false);

    iterator_wrapper<iterator<Creature, false>> iter(stay.begin());
    uint pos = 0;
    while(true) {
        int i=0;
        for(auto iter = stay.begin(); iter != stay.end(); ++iter, ++i)
            disp_inrow(to_disp(iter.getPointed()->getObject()), i, stay.size(), selected[i] ? SELECTED : 0);
        auto action = getNextPosition(&iter, pos, stay.size());
        if(action == change) selected[pos] = !selected[pos];
        else break;
    }
    auto iter2 = stay.begin();
    for(uint i=0; i<stay.size(); ++i){
        iter2.present();

        if(selected[i]) {
            iter2 = stay.splice(iter2);
        } else {
            ++iter2;
        }
    }
}
