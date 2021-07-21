#include "../HFiles/headC_constants.h"

const char olympus::trigger_types::PermETB = 0;
const char olympus::trigger_types::PermLTB = 1;
const char olympus::trigger_types::PermStateChange = 2;
const char olympus::trigger_types::PermBecomes = 3;
const char olympus::trigger_types::CreaAttacks = 4;
const char olympus::trigger_types::DamageableIsDamaged = 5;

const std::string olympus::trigger_descriptions[] = {"~ enters the battlefield",
"~ leaves the battlefield",
"~ changes state",
"~ becomes something",
"~ attacks",
"~ is dealt damage" };

const std::string olympus::ability_descriptions[] = {"~ deals ? damage to target opponent",
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

//! the dictionary sorts only lowercase letters and _.
//Uppercase might be tolerated for letters which are never checked, but as a general rule only use lowercase!

const char* const all_trigtypes[] = {
	"etb", "ltb", "statechange", "becomes", "attacks", "is_damaged"
}; const Dictionary olympus::dict_trigtypes(6, all_trigtypes);

const char* const all_abilities[] = {
	"ping", "draw", "add_life", "set_life",
	"add", "destroy", "exile", "counter",
	"p_power", "p_toughness", "put_counter", "untap"
}; const Dictionary olympus::dict_abiltypes(12, all_abilities);

const char* const static_descriptions[] = {
	"anthem"
}; const Dictionary olympus::dict_static_types(1, static_descriptions);

const char* const selector_descriptions[] = {
	"type",
	"subtype",
	"tribe",
	"ctrl"
}; const Dictionary olympus::dict_selectors(4, selector_descriptions);

const char* const selector_chars_type[] = {
	"player",
	"permanent",
	"resolvable",
	"card"
}; const Dictionary olympus::dict_selector_types(4, selector_chars_type);

const char* const selector_chars_subtype[] = {
	"creature",
	"artifact",
	"land",
	"planeswalker",
	"instant",
	"sorcery",
	"actability",
	"trigability"
}; const Dictionary olympus::dict_selector_subtypes(8, selector_chars_subtype);

const char* const selector_chars_tribe[] = {
	"human",
	"merfolk",
	"zombie",
	"goblin",
	"elf"
}; const Dictionary olympus::dict_selector_tribes(5, selector_chars_tribe);

const char* const selector_chars_players[] = {
        "you",
        "opponent"
}; const Dictionary olympus::dict_selector_players(2, selector_chars_players);