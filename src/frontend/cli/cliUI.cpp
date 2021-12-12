#include "lib3_cliUI.h"
#include "lib3_nanoIO.h"
#include "classes/perm_option.h"
#include "gameplay/2cards.h"
#include "gameplay/permanents/4permanents.h"
#include "gameplay/resolvables/stack.h"
#include "control/3player.h"

void CliUI::addCards(const std::list<CardWrapper>&) {}

void CliUI::registerPlayers(std::list<Player> &players) { the_players = &players; }

void CliUI::splitDamage(int power, std::list<std::pair<uint8_t, SpecificTargeter<Creature>>> &blockers) {
    (void) power; (void) blockers; //TODO
}

Target* CliUI::chooseTarget(char type) {
    (void) type; return nullptr;
}

bool CliUI::attackSwitch(int leftY, int rightY, int topZ, int arrowlength) const {
    (void) leftY; (void) rightY; (void) topZ; (void) arrowlength;
    return false;
}

bool CliUI::chooseattackers(Y_Hashtable<Creature> &cowards) {
    (void) cowards;
    return false;
}

void CliUI::chooseblockers(Y_Hashtable<Creature> &defenders, StateTN<Creature> &attackers) {
    (void) defenders; (void) attackers;
}

void CliUI::disp(fwdlist<uptr<Card>>::const_iterator begin, const fwdlist<uptr<Card>>::const_iterator end) {
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
            for(const auto& perm : pl->myboard) io.disp_inrow(&perm, i++, pl->myboard.size(), BasicIO::INLINE);
            break;
        case zone::stack:
            for(const auto& x : *Stack::god) io.disp_inrow(x.get(), i++, Stack::god->size(), BasicIO::INLINE);
            break;
        case zone::exile:
        case zone::commandzone:
        case zone::graveyard:
            Player::myzone zn = zone == zone::exile ? Player::exile : zone == zone::commandzone ? Player::command : Player::graveyard;
            for(const auto& card : pl->getZone(zn).getCards())
                io.disp_inrow(card.get(), i++, pl->getZone(zn).getSize(), BasicIO::INLINE );
    }
}

void CliUI::registerMe(Player* player) {
    pl = player;
}
