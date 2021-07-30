#include "1general.h"
#include "9modifs.h"
#include <iostream>

void Targeter::setTarget(Target* tar){
	content = tar;
	gdebug(DBG_TARGETING) << "Target chosen. this Targeter "<< this <<" was set to target"<<tar<<"\n";
	tar->add_persecutor(this);
	name = tar->get_name();
}

Targeter::Targeter(Target* tar): valid(true) {
    if(tar == nullptr){
        valid = false;
        content = nullptr;
    }
    else setTarget(tar);
}

void Target::add_persecutor(Targeter* tar){
	//god.gdebug(DBG_TARGETING) << "This Target "<<this<<" is the target of a Targeter" <<tar<<std::endl;
	to_target.push_front(tar);
}

void Target::remove_persecutor(Targeter* tar){
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
