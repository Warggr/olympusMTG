#ifndef OLYMPUS_YGGDRASIL_BOARDN_H
#define OLYMPUS_YGGDRASIL_BOARDN_H

#include "abstract_n.h"
#include "collection_tn.h"
#include "gameplay/permanents/4permanents.h"

template<typename T, bool iconst> class iterator;

class BoardN: public AbstractN {
public:
    CollectionTN<Land> mylands;
    CollectionTN<Artifact> myartos;
    CollectionTN<Planeswalker> mysuperfriends;
    CollectionTN<Creature> mycreas;
    StateTN<Creature>* myattackers {nullptr};

    BoardN(): mylands(this), myartos(this), mysuperfriends(this), mycreas(this) {};
    ~BoardN();

    void insert(Card* to_add, Player* pl);
    iterator<Permanent, false> pbegin();
    iterator<Permanent, false> pend() const;
    iterator<Permanent, true> cpbegin() const;
    iterator<Permanent, true> cpend() const;
};

#endif //OLYMPUS_YGGDRASIL_BOARDN_H
