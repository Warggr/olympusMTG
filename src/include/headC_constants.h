#ifndef OLYMPUS_HEADER_1_CONSTANTS_H
#define OLYMPUS_HEADER_1_CONSTANTS_H

#include <string>

namespace olympus{
	extern const std::string trigger_descriptions[6];
	extern const std::string ability_descriptions[12];

	namespace trigger_types{
		enum perm_trig { PermETB = 0, PermLTB, PermStateChange, PermBecomes };
		enum crea_trig { CreaAttacks = 0 };
		enum damageable_trig { DamageableIsDamaged = 0 };
	}
}

#endif //OLYMPUS_HEADER_1_CONSTANTS_H