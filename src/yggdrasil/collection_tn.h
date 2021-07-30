#ifndef OLYMPUS_YGGDRASIL_2_H
#define OLYMPUS_YGGDRASIL_2_H

#include "abstract_n.h"
#include "state_tn.h"

class BoardN;

template <typename T>
class CollectionTN: public AbstractN {
protected:
    BoardN* parent;
    std::list<StateTN<T>> children;
public:
    CollectionTN(BoardN* parent): parent(parent) {}
};

#endif //OLYMPUS_YGGDRASIL_2_H
