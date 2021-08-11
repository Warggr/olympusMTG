#include "4permanents.h"
#include "gameplay/1general.h"
#include "control/3player.h"

void Creature::hit(Damageable* tgt) {
	tgt->damage(get_power(), this);
}

void Creature::resolve_attack(Player* nextopponent){
    if(!is_block){
        hit(nextopponent);
    }
    else if(assigned_bl.size() == 1){
        Creature* blocker = assigned_bl.front().second.getTarget();
        if(blocker != nullptr){
            hit(blocker);
            blocker->hit(this);
        }
    }
    else{
        for(auto & itb : assigned_bl){
            (*itb.second)->damage(itb.first, this);
            itb.second->hit(this);
        }
    }
    assigned_bl.clear();
}

void Creature::add_blocker(Creature* bl){
    is_block = true;
    assigned_bl.emplace_front(std::pair<uint8_t, SpecificTargeter<Creature>>(0, bl));
}
