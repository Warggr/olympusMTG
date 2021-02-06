#ifndef OLYMPUS_HEADER_1_CONSTANTS_H
#define OLYMPUS_HEADER_1_CONSTANTS_H

#include <string>
#include "../Dictionary/head4_dictionary.h"

namespace olympus{
	extern const std::string trigger_descriptions[6];
	extern const std::string ability_descriptions[12];
	
	extern const Dictionary dict_trigtypes;
	extern const Dictionary dict_abiltypes;
	extern const Dictionary dict_static_types;
	extern const Dictionary dict_selectors;
	extern const Dictionary dict_selector_types;
	extern const Dictionary dict_selector_subtypes;
	extern const Dictionary dict_selector_tribes;
	extern const Dictionary dict_selector_players;

	namespace trigger_types{
		extern const char PermETB, PermLTB, PermStateChange, PermBecomes;
		extern const char CreaAttacks;
		extern const char DamageableIsDamaged;
	}
}

#endif //OLYMPUS_HEADER_1_CONSTANTS_H