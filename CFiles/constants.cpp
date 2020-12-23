#include ".header_link.h"

const int olympus::nb_trigtypes = 6;
const char* const olympus::all_trigtypes[] = {"etb", "ltb", "statechange", "becomes", "attacks", "is_damaged"}; //! the dictionary sorts only lowercase letters and _.
//Uppercase might be tolerated for letters which are never checked, but as a general rule only use lowercase!

const char olympus::trigger_types::PermETB = 0;
const char olympus::trigger_types::PermLTB = 1;
const char olympus::trigger_types::PermStateChange = 2;
const char olympus::trigger_types::PermBecomes = 3;
const char olympus::trigger_types::CreaAttacks = 4;
const char olympus::trigger_types::DamageableIsDamaged = 5;

const int olympus::nb_abilities = 12;
const char* const olympus::all_abilities[] = {"ping", "draw", "add_life", "set_life", "add", "destroy", "exile", "counter", "p_power", "p_toughness", "put_counter", "untap"};

const std::string Resolvable::description = "this spell or ability";

const std::string olympus::abilities_descriptions[] = {"~ deals ? damage to target opponent",
"Target player draws ? cards",
"Target player gains ? life",
"Your life total becomes ?",
"Add one mana of color ?",
"Destroy something",
"Exile something",
"Counter target spell or ability",
"Target creature gains +?/+0 until end of turn",
"Target creature gains +0/+? until end of turn",
"Put ? +1/+1 counters on target creature",
"Untap target permanent" };

const std::string olympus::trigger_descriptions[] = {"~ enters the battlefield",
"~ leaves the battlefield",
"changes state",
"becomes something",
"attacks",
"is dealt damage" };