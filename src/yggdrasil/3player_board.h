#ifndef OLYMPUS_YGGDRASIL_BOARDN_H
#define OLYMPUS_YGGDRASIL_BOARDN_H

#include "abstract_n.h"
#include "collection_tn.h"
#include <memory>

template<typename T, bool iconst> class iterator;

class BoardN: public Yggdrasil<Permanent> {
public:
    template<bool b>
    class myiterator: public inner_iterator<Permanent, b> {
        isitconst(BoardN, b)* par;
        char pos : 2;
    public:
        myiterator(isitconst(BoardN, b)* par, inner_iterator<Permanent, b>* parent = nullptr):
            inner_iterator<Permanent, b>(parent), par(par), pos(0) {};
        void advance() override { pos++; }
        bool isEnd() const override { return pos == 3; }

        Leaf<Permanent, b>* down() override {
            return (pos == 0) ? par->myartos.createStart(this) :
                (pos == 1) ? par->mycreas.pcreateStart(this) :
                    par->mysuperfriends.pcreateStart(this);
        }
    };

    CollectionTN<Permanent> myartos;
    CollectionTN<Planeswalker> mysuperfriends;
    CollectionTN<Creature> mycreas;
    StateTN<Creature>* myattackers {nullptr};

    BoardN(): myartos(this), mysuperfriends(this), mycreas(this) {};

    void insert(uptr<Card> to_add, Player* pl);
//    void remove(Permanent* perm, permanent_type type);

    void remove(Permanent* perm);
    void remove(Creature* crea);
    void remove(Planeswalker* plan);

    bool empty() const override { return myartos.empty() && mysuperfriends.empty() && mycreas.empty(); }

    iterator<Permanent, false> begin() override { return myartos.begin(); }
    iterator<Permanent, true> cbegin() const override { return myartos.cbegin(); }
    ConcreteLeaf<Permanent, false>* createStart(inner_iterator<Permanent, false>* parent) override {
        return myartos.createStart(new myiterator<false>(this, parent));
    }
    ConcreteLeaf<Permanent, true>* createStart(inner_iterator<Permanent, true>* parent) const override {
        return myartos.createStart(new myiterator<true>(this, parent));
    }
};

#endif //OLYMPUS_YGGDRASIL_BOARDN_H
