#include "headE_enums.h"
#include <array>

const std::array<const char*, 6> trigger_descriptions = {"~ enters the battlefield",
"~ leaves the battlefield",
"~ changes state",
"~ becomes something",
"~ attacks",
"~ is dealt damage" };

const std::array<const char*, 12> ability_descriptions = {"~ deals ? damage to target opponent",
"Target local draws ? cards",
"Target local gains ? life",
"Your life total becomes ?",
"Add one mana of color ?",
"Destroy something",
"Exile something",
"Counter target spell or ability",
"Target creature gains +?/+0 until end of turn",
"Target creature gains +0/+? until end of turn",
"Put ? +1/+1 counters on target creature",
"Untap target permanent" };
