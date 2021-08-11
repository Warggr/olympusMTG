#include "1general.h"
#include "9modifs.h"
#include <iostream>

void Target::add_persecutor(AbstractTargeter* tar){
	//god.gdebug(DBG_TARGETING) << "This Target "<<this<<" is the target of a Targeter" <<tar<<std::endl;
	to_target.push_front(tar);
}

void Target::remove_persecutor(AbstractTargeter* tar){
#ifdef F_STRICT
	gdebug(DBG_TARGETING) << "Removing: this Target "<<this<<" is not any longer harassed by"<<tar<<"\n";
	std::list<Targeter*>::iterator iter;
	for(iter = to_target.begin(); iter!=to_target.end(); iter++){
		if(*iter == tar) break;
	}
	if(iter == to_target.end()){
		gdebug(DBG_INTERNAL | DBG_IMPORTANT) << "Error: Tried to remove a Targeter from a Target that wasn't targeted by this target (cf. general.cpp)\n";
		exit(1);
	}
	else to_target.erase(iter);
#else
	to_target.remove(tar);
#endif
}

Target::~Target() {
    for(auto* persec : to_target) {
        persec->invalidate();
    }
}

namespace target_type {
    const flag_t tars1[] = { damageable, nonnegative };
    const flag_t tars2[] = { player, nonnegative };
    const flag_t tars3[] = { player, number };
    const flag_t tars4[] = { player, added_mana };
    const flag_t tars5[] = { permanent };
    const flag_t tars6[] = { resolvable };
    const flag_t tars7[] = { permanent, strictpositive };
    const flag_t tars8[] = { permanent };
    const flag_t* target_types[] = { tars1, tars2, tars3, tars3, tars4, tars5, tars5, tars6, tars7, tars8 };
    const int target_numbers[] = { 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1 };
}
