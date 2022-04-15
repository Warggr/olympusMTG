#ifndef OLYMPUS_TARGETERS_H
#define OLYMPUS_TARGETERS_H

//Represents an ability which has a target to affect, such as an enchantment's ability to Detain a creature, 
//or an Instant (on the stack)'s ability which will damage your opponent.
//Is (always, unless there are really weird exceptions) subordinate to a Spell or Permanent.
//We will use two types of lists of Modifs; output lists (all targets of a Spell or all creatures affected by an ability; length mostly constant);
//and input lists (all Modifs affecting a certain Targetable, which will all become invalid if the creature disappears or gains protection)
//In short: Resolvables contains (an array of) Targeters, and Targets contain (a list of) Targeter pointers
//Additionally, Triggers contain an Origin targeter, which is little more than a pointer. 
//such targeters are called 'internal targeters' and have actually no game relevance

#include <string>
#include <cassert>
#include "logging.h"

class Target;

class AbstractTargeter {
protected:
    bool valid;
public:
    explicit AbstractTargeter(bool valid): valid(valid) {};
    void invalidate() { valid = false; }
};

template<typename T>
class SpecificTargeter : public AbstractTargeter {
private:
    T* content;
    std::string name; //will be remembered even after the target dies
public:
    char colorIdentity; //-1 means it's an "internal" targeter, such as the one linking an ability to its origin.
    //you can't, of course, have protection from 'internal' targeters

    SpecificTargeter(T* tar): AbstractTargeter(true) {
        content = tar;
        gdebug(DBG_TARGETING) << "Target chosen. this Targeter "<< this <<" was set to target"<<tar<<"\n";
        tar->addPersecutor(this);
        name = tar->getName();
    }
    SpecificTargeter(): AbstractTargeter(false), colorIdentity(-1) { gdebug(DBG_TARGETING) << " Creating a Targeter\n"; }
    ~SpecificTargeter(){
        content->removePersecutor(this); //removes references to itself from its victim
    };
    void setTarget(T* tgt) {
        assert(tgt != nullptr);
        valid = true; content = tgt;
    }
    T* getTarget() const { if(valid) return content; else return nullptr; };
    const std::string& getName() const { return name; };
    T& operator*() { return *content; }
    T* operator->() { return content; }
    explicit operator bool() {
        return valid;
    }
};

using Targeter = SpecificTargeter<Target>;

#endif //OLYMPUS_TARGETERS_H
