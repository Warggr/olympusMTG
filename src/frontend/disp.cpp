#include "frontend/basicIO.h"
#include "classes/card_oracle.h"
#include "gameplay/permanents/4permanents.h"
#include "gameplay/2cards.h"
#include "gameplay/resolvables/5resolvables.h"
#include "control/3player.h"
#include "classes/perm_option.h"
#include <vector>

void CardOption::disp(Canvas* io, disp_flags flags) const {
    (void) io; (void) flags; //TODO
}

void CardOracle::disp(Canvas* io, disp_flags flags) const {
    io->disp(*this, flags);
}

void Permanent::disp(Canvas* io, disp_flags flags) const {
    io->disp(*source->oracle, flags);
}

void Spell::disp(Canvas* io, disp_flags flags) const {
    io->disp(*source->oracle, flags);
}

void Card::disp(Canvas* io, disp_flags flags) const {
    io->disp(*oracle, flags);
}

void Gamer::disp(Canvas* io, disp_flags flags) const {
    io->disp_player(*this, flags);
}

void Resolvable::disp(Canvas* io, disp_flags flags) const {
    (void) io; (void) flags; //TODO
}

void PermOption::disp(Canvas* io, disp_flags flags) const {
    (void) io; (void) flags; //TODO
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
    uint size = stay.size();
    gdebug(DBG_YGGDRASIL) << size << '\n';
    for(uint i=0; i<size; ++i){
        iter2.present();

        if(selected[i]) {
            iter2 = stay.splice(iter2);
        } else {
            ++iter2;
        }
    }
}
