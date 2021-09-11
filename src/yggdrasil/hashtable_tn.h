#ifndef OLYMPUS_YGGDRSAIL_H
#define OLYMPUS_YGGDRSAIL_H

#include "abstract_n.h"
#include "collection_tn.h"
class BoardN; class Player; class Card;

template<typename T>
class Y_Hashtable : public Yggdrasil<T> {
protected:
    BoardN* parent;
    int ht_size_log;
    CollectionTN<T>* ht;
public:
    template<bool b>
    class myiterator : public inner_iterator<T, b> {
        unsigned char position, order, multiplicity;
        isitconst(Y_Hashtable*, b) node;
    public:
        explicit myiterator(isitconst(Y_Hashtable*, b) n, inner_iterator<T, b>* parent = nullptr) :
            inner_iterator<T, b>(parent), position(0), order(1), multiplicity(0), node(n) {};
        void advance(bool bk) override { if(bk) position++; else position--; }
        bool isEnd(bool bk) const override { if(bk) return position * order > (1<<node->ht_size_log); else return position + 1 == 0; }
        Leaf<T, b>* down(bool bk) override { return node->createStart(this, bk); }
    };

    Y_Hashtable(BoardN* parent): parent(parent), ht_size_log(0), ht(new CollectionTN<T>[1])  { ht->setParent(this); }
    void unfold(StateTN<T>* newState) {
        auto* new_ht = new CollectionTN<T>[(1 << (ht_size_log+1))];
        for(int i=0; i<(1 << ht_size_log); i++) {
            new_ht[i] = std::move(ht[i]);
            new_ht[(1 << ht_size_log) + i].setParent(this);
        }
        ht_size_log++;
        delete[] ht;
        ht = new_ht;
        newState->init(ht_size_log, this);
    }
    void refold(int multiplicity) {
        auto* new_ht = new CollectionTN<T>[(1 << (ht_size_log-1))];
        for(int i = 0; i < (1 << (ht_size_log - multiplicity - 1)); ++i )
            for (int j = 0; j < (1 << multiplicity); ++j ) {
                new_ht[i * (1 << (multiplicity + 1)) + j] = std::move(ht[i * (1 << (multiplicity + 1)) + j]);
                new_ht[i * (1 << (multiplicity + 1)) + j].merge(ht[(2*i + 1) * (1 << multiplicity) + j]);
            }
        delete[] ht;
        ht = new_ht;
    }
    inline bool partlyEmpty(int multiplicity) const {
        for(int i = 0; i < (1 << (ht_size_log-multiplicity-1)); ++i )
            for (int j = 0; j < multiplicity; ++j )
                if (!ht[2*i*multiplicity + j].empty()) return false;
        return true;
    }
    bool empty() const override {
        return partlyEmpty(ht_size_log+1);
    }
    iterator<T, false> begin() override { return { createStart(nullptr, true) }; }
    iterator<T, true> cbegin() const override { return { createStart(nullptr, true) }; }
    iterator<Permanent, false> pbegin() override {
        return { new AdapterLeaf<T, false>( createStart(nullptr, true), nullptr) };
    }
    iterator<Permanent, true> cpbegin() const override {
        return { new AdapterLeaf<T, true>( createStart(nullptr, true), nullptr) };
    }
    ConcreteLeaf<T, false>* createStart(inner_iterator<T, false>* iter, bool bk) override {
        if(bk) {
            for (int i = 0; i < (1<<ht_size_log); ++i) if (!ht[i].empty()) {
                return ht[i].createStart(new myiterator<false>(this, iter), true);
            }
        } else {
            for (int i = (1<<ht_size_log)-1; i >= 0; --i) if (!ht[i].empty()) {
                return ht[i].createStart(new myiterator<false>(this, iter), false);
            }
        }
        return nullptr;
    }
    ConcreteLeaf<T, true>* createStart(inner_iterator<T, true>* iter, bool bk) const override {
        if(bk) {
            for (int i = 0; i < (1<<ht_size_log); ++i) if (!ht[i].empty()) {
                return ht[i].createStart(new myiterator<true>(this, iter), true);
            }
        } else {
            for (int i = (1<<ht_size_log)-1; i >= 0; --i) if (!ht[i].empty()) {
                return ht[i].createStart(new myiterator<true>(this, iter), false);
            }
        }
        return nullptr;
    }
    Leaf<Permanent, false>* pcreateStart(inner_iterator<Permanent, false>* iter, bool bk) override {
        return new AdapterLeaf<T, false>(createStart(nullptr, bk), iter);
    }
    Leaf<Permanent, true>* pcreateStart(inner_iterator<Permanent, true>* iter, bool bk) const override {
        return new AdapterLeaf<T, true>(createStart(nullptr, bk), iter);
    }

    void insert(std::unique_ptr<Card> origin, Player* pl) {
        (void) origin; (void) pl;
        //TODO
    }

    void remove(T* object) {
        (void) object; //TODO implement
    };

    friend class StateTN<T>;
};

/*class Y_Dichotomy : public Yggdrasil {
    Yggdrasil* parts[2] { nullptr };
public:
    class iterator : public inner_iterator {
        Y_Dichotomy* node;
        char pos : 1;
    public:
        iterator(Y_Dichotomy* node, inner_iterator* parent): inner_iterator(parent), node(node), pos(0) {}
        bool isEnd() const override { return pos == 1; }
        void advance() override { pos = 1; }
        Yggdrasil& getPointed() override { return *node->parts[pos]; }
    };

    Yggdrasil::iterator begin() override {
        return Yggdrasil::iterator(new iterator(this, nullptr));
    }
    inner_iterator* createStart(inner_iterator *parent) override {
        return parts[0]->createStart(new iterator(this, parent));
    }
};*/

#endif //OLYMPUS_YGGDRSAIL_H
