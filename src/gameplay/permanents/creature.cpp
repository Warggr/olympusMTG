#include "4permanents.h"
#include "gameplay/1general.h"

void Creature::hit(Damageable* tgt){
	tgt->damage(get_power(), this);
}

const std::string Permanent::describe() const {
	return source->oracle->describe();
}

void Creature::resolve_attack(Player* nextopponent){
    if(!is_block){
        hit(nextopponent);
    }
    else if(assigned_bl.size() == 1){
        Creature* blocker = assigned_bl.front().getTarget();
        if(blocker != 0){
            hit(blocker);
            blocker->hit(this);
        }
    }
    else{
        getController()->getAgent()->splitDamage(get_power(), assigned_bl);
    }
    assigned_bl.clear();
}

void Creature::add_blocker(Creature* bl){
    is_block = true;
    assigned_bl.push_front(bl);
}
