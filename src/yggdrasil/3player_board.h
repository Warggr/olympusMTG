#ifndef OLYMPUS_YGGDRASIL_BOARDN_H
#define OLYMPUS_YGGDRASIL_BOARDN_H

#include "abstract_n.h"
#include "hashtable_tn.h"
#include "state_n.h"
#include <memory>

class Permanent; class Planeswalker; class Creature; class Player; class MasterN;

class BoardN: public Yggdrasil<Permanent> {
public:
    template<bool b>
    class myiterator: public inner_iterator<Permanent, b> {
        isitconst(BoardN, b)* par;
        unsigned char pos : 2;
    public:
        myiterator(isitconst(BoardN, b)* par, inner_iterator<Permanent, b>* parent = nullptr, char starting_pos = 0):
            inner_iterator<Permanent, b>(parent), par(par), pos(starting_pos) {};

        void advance(bool bk) override { if(bk) pos++; else pos--; }
        bool isEnd(bool) const override { return pos == 3; }

        Leaf<Permanent, b>* down(bool bk) override {
            return (pos == 0) ? par->myartos.createStart(this, bk) :
                (pos == 1) ? par->mycreas.pcreateStart(this, bk) :
                    par->mysuperfriends.pcreateStart(this, bk);
        }
        void present(uint indent, logging::record_ostream& strm) const override {
            if(iterator_treenode<Permanent, b>::parent) iterator_treenode<Permanent, b>::parent->present(indent + 1, strm);
            for(uint i = 0; i<indent; i++) strm << '>';
            strm << "BoardN @" << par << " at pos " << static_cast<int>(pos) << '\n';
        }
    };

    MasterN* parent; //TODO FEATURE initialize this

    Y_Hashtable<Permanent> myartos;
    Y_Hashtable<Planeswalker> mysuperfriends;
    Y_Hashtable<Creature> mycreas;
    StateTN<Creature> myattackers;

    BoardN(): myartos(*this), mysuperfriends(*this), mycreas(*this) {};

    void insert(card_ptr to_add, Player* pl);
//    void remove(Permanent* perm, permanent_type type);

    void remove(Permanent* perm);
    void remove(Creature* crea);
    void remove(Planeswalker* plan);

    bool empty() const override { return myartos.empty() && mysuperfriends.empty() && mycreas.empty(); }
    unsigned int size() const override { return myartos.size() + mysuperfriends.size() + mycreas.size(); }

    iterator<Permanent, false> begin() override { return { createStart(nullptr, true) }; }
    iterator<Permanent, true> cbegin() const override { return { createStart(nullptr, true) }; }

    ConcreteLeaf<Permanent, false>* createStart(inner_iterator<Permanent, false>* parent, bool bk) override {
        Leaf<Permanent, false>* ret = myartos.createStart(new myiterator<false>(this, parent), bk);
        if(!ret) ret = AdapterLeaf<Creature, false>::create(
                mycreas.createStart(nullptr, bk),
                new myiterator<false>(this, parent, 1));
        if(!ret) ret = AdapterLeaf<Planeswalker, false>::create(
                mysuperfriends.createStart(nullptr, bk),
                new myiterator<false>(this, parent, 2));
        return static_cast<ConcreteLeaf<Permanent, false>*>(ret); //we're gonna downcast this again in pcreatestart
    }
    ConcreteLeaf<Permanent, true>* createStart(inner_iterator<Permanent, true>* parent, bool bk) const override {
        Leaf<Permanent, true>* ret = myartos.createStart(new myiterator<true>(this, parent), bk);
        if(!ret) ret = AdapterLeaf<Creature, true>::create(
                mycreas.createStart(nullptr, bk),
                new myiterator<true>(this, parent, 1));
        if(!ret) ret = AdapterLeaf<Planeswalker, true>::create(
                mysuperfriends.createStart(nullptr, bk),
                new myiterator<true>(this, parent, 2));
        return static_cast<ConcreteLeaf<Permanent, true>*>(ret); //we're gonna downcast this again in pcreatestart
    }

    void disp(unsigned int indent, logging::record_ostream& strm) const override {

        for(uint i=0; i<indent; i++) strm << ' ';
        strm << "---BoardN @" << this << '\n';

        for(uint i=0; i<indent; i++) strm << ' ';
        strm << "Permanents:\n";
        myartos.disp(indent+1, strm);

        for(uint i=0; i<indent; i++) strm << ' ';
        strm << "Creatures:\n";
        mycreas.disp(indent+1, strm);

        for(uint i=0; i<indent; i++) strm << ' ';
        strm << "Planeswalkers:\n";
        mysuperfriends.disp(indent+1, strm);

        for(uint i=0; i<indent; i++) strm << ' ';
        strm << "Attackers:\n";
        myattackers.disp(indent+1, strm);
    }
};

#endif //OLYMPUS_YGGDRASIL_BOARDN_H
