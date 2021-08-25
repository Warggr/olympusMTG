#include "oracles/classes/PermOption.h"
#include "oracles/classes/8options.h"

void PermOption::fillout(Mana c, Effect_H* preRes, bool ts, bool ismana){
    cost = c; effects = preRes; tapsymbol = ts; ismanaability = ismana;
}

SpellOption::SpellOption(bool isLand, bool instantSpeed): Option(0, instantSpeed), isLand(isLand) {}

SpellOption::SpellOption(card_type type): Option(0, type.underlying == card_type::instant), isLand(type.land) {}

void CardOption::init(ReaderVisitor &visitor) {
//TODO
}

PermOption::~PermOption() = default;