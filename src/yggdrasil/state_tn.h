#ifndef OLYMPUS_STATE_TN_H
#define OLYMPUS_STATE_TN_H

#include "abstract_n.h"
#include "permanent_tn.h"

template<typename T> class CollectionTN;

template <typename T>
class StateTN: public AbstractN {
    std::list<CollectionTN<T>*> parents;
    std::list<PermanentTN<T>> children;
public:
    StateTN(Card* source, Player* pl, CollectionTN<T>* parent) {
        parents.push_front(parent);
        children.emplace_front(source, pl, this);
    }
};

#endif //OLYMPUS_STATE_TN_H
