#ifndef OLYMPUS_STATE_TN_H
#define OLYMPUS_STATE_TN_H

#include "abstract_n.h"
#include "permanent_tn.h"

template<typename T> class CollectionTN;

template <typename T>
class StateTN: public AbstractN {
    std::list<CollectionTN<T>*> parents;
    PermanentTN<T>* first, * last;
public:
    explicit StateTN(CollectionTN<T>* parent) {
        parents.push_front(parent);
    }
    StateTN(std::unique_ptr<Card> source, Player* pl, CollectionTN<T>* parent) {
        parents.push_front(parent);
        auto* inserat = new PermanentTN<T>(std::move(source), pl, this);
        if(first != nullptr) {
            inserat->place_before(first);
        }
        first = inserat;
    }

    void merge(StateTN<T>& other) {
        last->place_before(other.first);
    }

    bool empty() const override {
        return first == nullptr;
    }

    template<typename X> iterator<X, false> xbegin(){
        return iterator<X, false>(first, true);
    }
    
    template<typename X> iterator<X, true> cxbegin() const {
        return iterator<X, true>(first, true);
    }

    template<typename X, bool iconst> iterator<X, iconst> xend() const {
        return last->template xend<X, iconst>();
    }
    
    iterator<T, false> begin() { return xbegin<T>(); }
    iterator<T, true> cbegin() const { return cxbegin<T>(); }
    iterator<Permanent, false> pbegin() { return xbegin<Permanent>(); }
    iterator<Permanent, true> cpbegin() const { return cxbegin<Permanent>(); }
    iterator<T, false> end() const { return xend<T, false>(); }
    iterator<T, true> cend() const { return xend<T, true>(); }
    iterator<Permanent, false> pend() const { return xend<Permanent, false>(); }
    iterator<Permanent, true> cpend() const { return xend<Permanent, true>(); }
};

#endif //OLYMPUS_STATE_TN_H
