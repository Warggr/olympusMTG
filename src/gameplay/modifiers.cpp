#include "1general.h"
#include "9modifs.h"

void Target::addPersecutor(AbstractTargeter* tar){
    // gdebug(DBG_TARGETING) << "This Target "<<this<<" is the target of a Targeter" <<tar<<std::endl;
    to_target.push_front(tar);
}

void Target::removePersecutor(AbstractTargeter* tar){
#ifdef F_STRICT
    gdebug(DBG_TARGETING) << "Removing: this Target "<<this<<" is not any longer harassed by"<<tar<<"\n";
    std::list<Targeter*>::iterator iter;
    for(iter = to_target.begin(); iter!=to_target.end(); iter++){
        if(*iter == tar) break;
    }
    if(iter == to_target.end()){
        gdebug(DBG_TARGETING) << "Error: Tried to remove a Targeter from a Target that wasn't targeted by this target\n";
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
