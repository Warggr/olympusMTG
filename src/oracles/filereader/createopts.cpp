#include "oracles/classes/PermOption.h"
#include "oracles/classes/8options.h"

SpellOption::SpellOption(bool isLand, bool instantSpeed): Option(0, instantSpeed), isLand(isLand) {}

SpellOption::SpellOption(card_type type): Option(0, type.isInstant()), isLand(type.land) {}

void CardOption::init(ReaderVisitor &visitor) {
    (void) visitor; //TODO
}

PermOption::~PermOption() = default;
