#ifndef OLYMPUS_PERMANENT_TN_H
#define OLYMPUS_PERMANENT_TN_H

#include "abstract_n.h"
template<typename T> class StateTN; class Player;

class AbPermanentN {
public:
    AbPermanentN* pprev, * pnext;
    virtual const Permanent& get_describedObject(const Permanent* signature_compl) const = 0;
    virtual Permanent& get_describedObject(Permanent* signature_compl) = 0;
};

//the base class is the PermanentTN. It contains (and owns) a permanent.
template <typename T>
class PermanentTN: public AbstractN, public AbPermanentN {
    T describedObject;
    std::list<StateTN<T>*> parents;
    PermanentTN<T>* prev, * next; //PermanentTNs are doubly-linked in order to have fast iterating)
public:
    PermanentTN(std::unique_ptr<Card> source, Player* pl, StateTN<T>* par): describedObject(std::move(source), pl) {
        parents.push_front(par);
    }
    //Guideline: linking is always initiated from prev to next (next = x, next->prev = this)
    void place_before(PermanentTN<T>* nxt) {
        prev = nxt->prev;
        next = nxt;
        nxt->prev = this;
    }
    void unlink() {
        if(prev) prev->next = next;
        if(next) next->prev = prev;
    }

    //void unstate(){ single_out(); this->primary_parent->state_out(this); }; //changes state
    //void add(T* per);
    //void single_out(); //removes object from its siblings
    //void obliterate(){single_out(); delete this; }; //unconnects from siblings & parents, signal its death to its parents (TODO) and deletes itself

    bool empty() const override { return false; }
    const Permanent& get_describedObject(const Permanent* signature_compl) const override {
        (void) signature_compl; return describedObject;
    }
    Permanent& get_describedObject(Permanent* signature_compl) override {
        (void) signature_compl; return describedObject;
    }

    template<typename X, bool iconst> friend class iterator;

    template<typename X, bool iconst> iterator<X, iconst> xbegin() const {
        return iterator<X, iconst>(this, true);
    }
    template<typename X, bool iconst> iterator<X, iconst> xend() const {
        return iterator<X, iconst>(next, true);
    }
};

#endif //OLYMPUS_PERMANENT_TN_H
