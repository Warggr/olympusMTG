#include "gameplay/resolvables/5resolvables.h"
#include "classes/1effects.h"
#include "control/3player.h"
#include "gameplay/gamer.h"
#include "headE_enums.h"

void Gamer::addMana(char c){
    manapool += c;
}

void Gamer::emptyPool(){
    manapool = Mana();
}

void Damageable::damage(int nb_damage, Target* origin){
    life -= nb_damage;
    is_damaged.trigger(getDmgController(), getMeAsTarget());
    origin->deals_damage.trigger(origin->getController(), nullptr);
}

void Effect_H::activate(SpecificTargeter<Target>* list_of_targets, Player* ctrl, Target* origin) const {
    for(auto& atom : effects) {
        atom.activate(list_of_targets, ctrl, origin);
    }
}

template<typename T>
inline T& as(Target* t){
    T* ret = dynamic_cast<T*>(t);
    assert(ret != nullptr);
    return *ret;
}

void AtomEffect_H::activate(Targeter* list_of_targets, Player* ctrl, Target* origin) const {
    using namespace effect;
    Target* target1;
    if(params[0] == targets::you){
        target1 = ctrl;
    } else if (params[0] == targets::origin) {
        target1 = origin;
    } else {
        target1 = list_of_targets[params[0]-3].getTarget();
    }
    switch(type){
    #define AS(Type) assert(dynamic_cast<Type*>(target1) != nullptr); dynamic_cast<Type*>(target1)
        case deal_damage:
            as<Damageable>(target1).damage(params[1], origin); break;
        case draw:
            as<Player>(target1).draw(params[1]); break;
        case gain_life:
            as<Player>(target1).gainLife(params[1]); break;
        case set_life:
            as<Gamer>(target1).setLife(params[1]); break;
        case add_mana:
            as<Gamer>(target1).addMana(params[1]); break;
        case destroy:
            as<Permanent>(target1).destroy(); break;
        case exile:
            as<Permanent>(target1).exile(); break;
        case counter:
            as<Resolvable>(target1).counter(); break;
        case untap:
            as<Permanent>(target1).untap(); break;
        default:
            gdebug(DBG_INTERNAL) << "Internal error: this ability (" << static_cast<int>(type) <<") hasn't been implemented yet\n";
    }
}

//To understand the use of the Target* origin, a quick example: Niv-Mizzet, the Firemind. His ability "ping 1" goes on the stack.
//When the ability resolves, if Niv's still alive, you will draw a card. If he's not, you won't.
//mostly, origin will have been drawn from a Targeter

/*ALL POSSIBLE ABILITIES
0 deal damage (Damageable target, unsigned int nb_dam)
1 draw (Player target, int nb_cards)
2 gain/lose life (Player target, int nb_life)
3 set life to (Player target, int nb_life)
4 add mana(Player target, int mana)
5 destroy (Permanent target)
6 counter (Resolvable target) //we could add sth like (Resolvable target, Mana unless_pay_amount)
+X/+0 (Creature target, int x)
+0/+X (Creature target, int x)
destroy_all(characteristics)
myExile (Permanent target)
copy (Resolvable target)
deal damage to all (characterstics, int nb_damage)
create token (Permanent to_copy)
//okay, so for tokens, a blank token will be created, then either an object will be copied or stats/abilities will be given.
make creature ?/X
make creature X/?
make local sacrifice
search myLibrary for a (characteristics)
make fight (Creature 1, Creature 2)
swap life (Player 1, Player 2)
gain control of (Player target) */
