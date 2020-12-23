#ifndef OLYMPUS_HEADER_1_CONSTANTS_H
#define OLYMPUS_HEADER_1_CONSTANTS_H

#include <string>

namespace olympus{
	extern const int nb_trigtypes;
	extern const char* const all_trigtypes[6]; //! the dictionary sorts only lowercase letters and _.
	//Uppercase might be tolerated for letters which are never checked, but as a general rule only use lowercase!
	extern const std::string trigger_descriptions[6];

	namespace trigger_types{
		extern const char PermETB, PermLTB, PermStateChange, PermBecomes;
		extern const char CreaAttacks;
		extern const char DamageableIsDamaged;
	}

	namespace directions{
		const char UP = -2, DOWN = 2, LEFT = -1, RIGHT = 1, ENTER = 3, SPACE = -3, MOUSE = 4, NOTONOBJECT = -4;
	}

	extern const int nb_abilities;
	extern const char* const all_abilities[12];

	extern const std::string abilities_descriptions[12];
}

#endif //OLYMPUS_HEADER_1_CONSTANTS_H