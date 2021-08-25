#ifndef OLYMPUS_YGGDRASIL_BOARDN_H
#define OLYMPUS_YGGDRASIL_BOARDN_H

#include "abstract_n.h"
#include "collection_tn.h"
#include "gameplay/permanents/4permanents.h"
#include <memory>

template<typename T, bool iconst> class iterator;

class BoardN: public AbstractN {
public:
    CollectionTN<Land> mylands;
    CollectionTN<Artifact> myartos;
    CollectionTN<Planeswalker> mysuperfriends;
    CollectionTN<Creature> mycreas;
    StateTN<Creature>* myattackers {nullptr};

    BoardN(): mylands(this), myartos(this), mysuperfriends(this), mycreas(this) {};

    bool empty() const override { return mylands.empty() && myartos.empty() && mysuperfriends.empty() && mycreas.empty(); }

    void insert(uptr<Card> to_add, Player* pl);
//    void remove(Permanent* perm, permanent_type type);

    iterator<Permanent, false> pbegin() { return mylands.pbegin(); }
    iterator<Permanent, false> pend() const { return mycreas.pend(); }
    iterator<Permanent, true> cpbegin() const { return mylands.cpbegin(); }
    iterator<Permanent, true> cpend() const { return mycreas.cpend(); }

    iterator<Permanent, false> begin() { return pbegin(); }
    iterator<Permanent, false> end() const { return pend(); }

    void remove(Permanent* perm) {
        switch(perm->getType()) {
            case permanent_type::creature: remove(dynamic_cast<Creature*>(perm)); break;
            case permanent_type::artifact: remove(dynamic_cast<Artifact*>(perm)); break;
            case permanent_type::planeswalker: remove(dynamic_cast<Planeswalker*>(perm)); break;
            case permanent_type::land: remove(dynamic_cast<Land*>(perm)); break;
        }
    }
    void remove(Creature* crea);
    void remove(Artifact* arti);
    void remove(Planeswalker* plan);
    void remove(Land* land);
};

#endif //OLYMPUS_YGGDRASIL_BOARDN_H
