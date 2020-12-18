#ifndef OLYMPUS_HEADER_CONSTANTS_H
#define OLYMPUS_HEADER_CONSTANTS_H

#define DBG_IMPORTANT 0x1
#define DBG_READFILE 0x2
#define DBG_X_READFILE 0x4
#define DBG_INTERNAL 0x8
#define DBG_TARGETING 0x10
#define DBG_RAGNAROK 0x20

namespace olympus{
	extern const int nb_trigtypes;
	extern const char* const all_trigtypes[6]; //! the dictionary sorts only lowercase letters and _.
	//Uppercase might be tolerated for letters which are never checked, but as a general rule only use lowercase!
	extern const std::string trigger_descriptions[6];

	namespace trigger_types{
		extern const char PermETB;
		extern const char PermLTB;
		extern const char PermStateChange;
		extern const char PermBecomes;
		extern const char CreaAttacks;
		extern const char DamageableIsDamaged;
	}

	namespace directions{
		const int UP = -2;
		const int DOWN = 2;
		const int LEFT = -1;
		const int RIGHT = 1;
		const int ENTER = 3;
		const int SPACE = -3;
	}

	extern const int nb_abilities;
	extern const char* const all_abilities[10];

	extern const std::string abilities_descriptions[10];
}

#endif //OLYMPUS_HEADER_CONSTANTS_H
