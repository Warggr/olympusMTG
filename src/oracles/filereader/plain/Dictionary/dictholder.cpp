#include "dictholder.h"

//! the dictionary sorts only lowercase letters and _.
//Uppercase might be tolerated for letters which are never checked, but as a general rule only use lowercase!

#define DICTIZ(x) (sizeof (x) / sizeof(const char*)) , x

const char* const all_trigtypes[] = {
        "etb", "ltb", "statechange", "becomes", "attacks", "is_damaged"
};

const char* const all_abilities[] = {
        "ping", "draw", "add_life", "set_life",
        "add", "destroy", "myExile", "counter",
        "p_power", "p_toughness", "put_counter", "untap"
};

const char* const static_descriptions[] = {
        "anthem"
};

const char* const selector_descriptions[] = {
        "type",
        "subtype",
        "tribe",
        "ctrl"
};

const char* const selector_chars_type[] = {
        "local",
        "permanent",
        "resolvable",
        "card"
};

const char* const selector_chars_subtype[] = {
        "creature",
        "artifact",
        "land",
        "planeswalker",
        "instant",
        "sorcery",
        "actability",
        "trigability"
};

const char* const selector_chars_tribe[] = {
        "human",
        "merfolk",
        "zombie",
        "goblin",
        "elf"
};

const char* const selector_chars_players[] = {
        "you",
        "opponent"
};

DictHolder::DictHolder():
        dict_trigtypes(DICTIZ(all_trigtypes)),
        dict_abiltypes(DICTIZ(all_abilities)),
        dict_static_types(DICTIZ(static_descriptions)),
        dict_selectors(DICTIZ(selector_descriptions)),
        dict_selector_types(DICTIZ(selector_chars_type)),
        dict_selector_subtypes(DICTIZ(selector_chars_subtype)),
        dict_selector_tribes(DICTIZ(selector_chars_players)),
        dict_selector_players(DICTIZ(selector_chars_players))
{}
