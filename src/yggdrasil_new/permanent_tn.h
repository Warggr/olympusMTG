#ifndef OLYMPUS_PERMANENT_TN_H
#define OLYMPUS_PERMANENT_TN_H

#include "abstract_n.h"
template<typename T> StateTN;

template <typename T>
class PermanentTN: public AbstractN { //the base class is the PermanentTN. It contains a permanent.
    T describedObject;
    std::list<StateTN<T>*> parents;
public:
    PermanentTN(Card* source, Player* pl, StateTN<T>* par): describedObject(source, pl) {
        parents.push_front(par);
    }

    //void unstate(){ single_out(); this->primary_parent->state_out(this); }; //changes state
    //void add(T* per);
    //void single_out(); //removes object from its siblings
    //void obliterate(){single_out(); delete this; }; //unconnects from siblings & parents, signal its death to its parents (TODO) and deletes itself

    const Permanent& get_describedObject(const Permanent* signature_compl) const { return describedObject; };
    Permanent& get_describedObject(Permanent* signature_compl) { return describedObject; };

    //template<typename X, bool iconst> friend class iterator;
};

#endif //OLYMPUS_PERMANENT_TN_H
