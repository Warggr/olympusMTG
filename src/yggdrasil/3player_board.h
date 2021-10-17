#ifndef OLYMPUS_YGGDRASIL_BOARDN_H
#define OLYMPUS_YGGDRASIL_BOARDN_H

#include "abstract_n.h"
#include "hashtable_tn.h"
#include "state_n.h"
#include <memory>
class Permanent; class Planeswalker; class Creature; class Player;

class BoardN: public Yggdrasil<Permanent> {
public:
    template<bool b>
    class myiterator: public inner_iterator<Permanent, b> {
        isitconst(BoardN, b)* par;
        char pos : 2;
    public:
        myiterator(isitconst(BoardN, b)* par, inner_iterator<Permanent, b>* parent = nullptr):
            inner_iterator<Permanent, b>(parent), par(par), pos(0) {};

        void advance(bool bk) override { if(bk) pos++; else pos--; }
        bool isEnd(bool) const override { return pos == 3; }

        Leaf<Permanent, b>* down(bool bk) override {
            return (pos == 0) ? par->myartos.createStart(this, bk) :
                (pos == 1) ? par->mycreas.pcreateStart(this, bk) :
                    par->mysuperfriends.pcreateStart(this, bk);
        }
    };

    Y_Hashtable<Permanent> myartos;
    Y_Hashtable<Planeswalker> mysuperfriends;
    Y_Hashtable<Creature> mycreas;
    StateTN<Creature> myattackers;

    BoardN(): myartos(this), mysuperfriends(this), mycreas(this) {};

    void insert(uptr<Card> to_add, Player* pl);
//    void remove(Permanent* perm, permanent_type type);

    void remove(Permanent* perm);
    void remove(Creature* crea);
    void remove(Planeswalker* plan);

    bool empty() const override { return myartos.empty() && mysuperfriends.empty() && mycreas.empty(); }

    iterator<Permanent, false> begin() override { return myartos.begin(); }
    iterator<Permanent, true> cbegin() const override { return myartos.cbegin(); }

    ConcreteLeaf<Permanent, false>* createStart(inner_iterator<Permanent, false>* parent, bool bk) override {
        Leaf<Permanent, false>* ret = myartos.createStart(new myiterator<false>(this, parent), bk);
        if(!ret) ret = new AdapterLeaf<Creature, false>(mycreas.createStart(nullptr, bk), parent);
        if(!ret) ret = new AdapterLeaf<Planeswalker, false>(mysuperfriends.createStart(nullptr, bk), parent);
        return static_cast<ConcreteLeaf<Permanent, false>*>(ret); //we're gonna downcast this again in pcreatestart
    }
    ConcreteLeaf<Permanent, true>* createStart(inner_iterator<Permanent, true>* parent, bool bk) const override {
        Leaf<Permanent, true>* ret = myartos.createStart(new myiterator<true>(this, parent), bk);
        if(!ret) ret = new AdapterLeaf<Creature, true>(mycreas.createStart(nullptr, bk), parent);
        if(!ret) ret = new AdapterLeaf<Planeswalker, true>(mysuperfriends.createStart(nullptr, bk), parent);
        return static_cast<ConcreteLeaf<Permanent, true>*>(ret); //we're gonna downcast this again in pcreatestart
    }
};

#endif //OLYMPUS_YGGDRASIL_BOARDN_H