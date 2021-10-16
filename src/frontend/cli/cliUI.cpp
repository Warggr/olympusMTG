#include "lib3_cliUI.h"
#include "lib3_nanoIO.h"
#include "oracles/classes/perm_option.h"
#include "gameplay/2cards.h"

void CliUI::addCards(const std::list<std::unique_ptr<Card>> &) {}

void CliUI::registerPlayers(std::list<Player> &players) { the_players = &players; }

void CliUI::splitDamage(int power, std::list<std::pair<uint8_t, SpecificTargeter<Creature>>> &blockers) {
    (void) power; (void) blockers; //TODO
}

uptr<OptionAction> CliUI::chooseOpt(bool sorcerySpeed) {
    (void) sorcerySpeed; return nullptr;
}

Target *CliUI::chooseTarget(char type) {
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
        io.disp(*i, NanoIO::INLINE);
    }
}
