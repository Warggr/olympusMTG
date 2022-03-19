#include "lib3_cliUI.h"
#include "lib3_nanoIO.h"
#include "classes/perm_option.h"
#include "gameplay/2cards.h"
#include "gameplay/permanents/4permanents.h"
#include "gameplay/resolvables/stack.h"
#include "control/3player.h"
#include "yggdrasil/iterators.h"

void CliUI::addCards(const std::list<CardWrapper>&) {}

void CliUI::splitDamage(int power, std::list<std::pair<uint8_t, SpecificTargeter<Creature>>>& blockers) {
    (void) power; (void) blockers; //TODO
}

const Target* CliUI::chooseTarget(char type) {
    (void) type; return nullptr;
}

void CliUI::chooseblockers(Y_Hashtable<Creature>& defenders, StateTN<Creature>& attackers) {
    (void) defenders; (void) attackers;
}

void CliUI::disp(fwdlist<card_ptr>::const_iterator begin, const fwdlist<card_ptr>::const_iterator end) {
    for(auto i = begin; i != end; ++i) {
        io.disp(**i, NanoIO::INLINE);
    }
}

void CliUI::list(zone::zone zone) {
    int i = 0;
    switch (zone) {
        case zone::hand:
            for(const auto& card : pl->getHand()) io.disp_inrow(card.get(), i++, pl->getHand().size(), BasicIO::INLINE);
            break;
        case zone::battlefield:
            for(auto perm = pl->myboard.cbegin(); perm != pl->myboard.end(); ++perm) io.disp_inrow(&(*perm), i++, pl->myboard.size(), BasicIO::INLINE);
            break;
        case zone::stack:
            for(const auto& x : *Stack::god) io.disp_inrow(x.get(), i++, Stack::god->size(), BasicIO::INLINE);
            break;
        case zone::exile:
        case zone::commandzone:
        case zone::graveyard:
            Player::myzone zn = zone == zone::exile ? Player::exile : zone == zone::commandzone ? Player::command : Player::graveyard;
            for(const auto& card : pl->getZone(zn).getCards())
                io.disp_inrow(raw_cardptr(card), i++, pl->getZone(zn).getSize(), BasicIO::INLINE );
    }
}

void CliUI::registerMe(const Gamer* player) {
    pl = player;
}
