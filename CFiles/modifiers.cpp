#include "../HFiles/classes.h"
#include <cstdlib>

void Targeter::setTarget(Target* tar){
	content = tar;
	god.gdebug(DBG_TARGETING) << "Target chosen. this Targeter "<< this <<" was set to target"<<tar<<std::endl;
	tar->add_persecutor(this);
}

void Target::add_persecutor(Targeter* tar){
	god.gdebug(DBG_TARGETING) << "This Target "<<this<<" is the target of a Targeter" <<tar<<std::endl;
	first_to_target = new ModListNode(tar, first_to_target);
}

void Target::remove_persecutor(Targeter* tar){
	god.gdebug(DBG_TARGETING) << "Removing: this Target "<<this<<" is not any longer harassed by"<<tar<<std::endl;
	ModListNode* iter;
	for(iter = first_to_target; iter!=0; iter = iter->next){
		if(iter->content == tar) break;
	}
	if(iter == 0){
		god.gdebug(DBG_INTERNAL | DBG_IMPORTANT) << "Error: Tried to remove a Targeter from a Target that wasn't targeted by this target (cf. modifiers.cpp)\n";
		exit(1);
	}
	if(iter->next) iter->next->prev = iter->prev;
	if(iter->prev) iter->prev->next = iter->next;
	else first_to_target = 0;
	delete iter;
}