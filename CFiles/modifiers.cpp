#include "../include/.olympus_main.h"
#include "../include/1general.h"
#include "../HFiles/9modifs.h"

void Targeter::setTarget(Target* tar){
	content = tar;
	god.gdebug(DBG_TARGETING) << "Target chosen. this Targeter "<< this <<" was set to target"<<tar<<std::endl;
	tar->add_persecutor(this);
	name = tar->get_name();
}

Targeter::Targeter(Target* tar): valid(true) {
    if(tar == 0){
        valid = false;
        content = 0;
    }
    else setTarget(tar);
}

void Target::add_persecutor(Targeter* tar){
	//god.gdebug(DBG_TARGETING) << "This Target "<<this<<" is the target of a Targeter" <<tar<<std::endl;
	to_target.push_front(tar);
}

void Target::remove_persecutor(Targeter* tar){
	god.gdebug(DBG_TARGETING) << "Removing: this Target "<<this<<" is not any longer harassed by"<<tar<<std::endl;
	std::list<Targeter*>::iterator iter;
	for(iter = to_target.begin(); iter!=to_target.end(); iter++){
		if(*iter == tar) break;
	}
	if(iter == to_target.end()){
		god.gdebug(DBG_INTERNAL | DBG_IMPORTANT) << "Error: Tried to remove a Targeter from a Target that wasn't targeted by this target (cf. modifiers.cpp)\n";
		exit(1);
	}
	else to_target.erase(iter);
}