#include "frontend/basicIO.h"
#include "permanents/4permanents.h"
#include "2cards.h"
#include "resolvables/5resolvables.h"
#include "control/3player.h"
#include "oracles/classes/perm_option.h"

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

void CardOracle::disp(BasicIO *io) const {
    io->disp(*this, 0);
}

void PermOption::disp(BasicIO *io) const {
    (void) io; //TODO
}
