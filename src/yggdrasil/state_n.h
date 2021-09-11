#ifndef OLYMPUS_STATE_N_H
#define OLYMPUS_STATE_N_H

#include "abstract_n.h"
#include "hashtable_tn.h"

template<typename T>
class StateTN: public Yggdrasil<T> {
    Y_Hashtable<T>* parent;
    int multiplicity;
public:
    template<bool b> class myiterator: public inner_iterator<T, b> {
        isitconst(StateTN<T>, b)* pted;
        int block, position;
    public:
        myiterator(isitconst(StateTN<T>, b)* pted, inner_iterator<T, b>* parent):
            inner_iterator<T, b>(parent), pted(pted), block(0), position(0) {};
        bool isEnd(bool bk) const override {
            if(bk) return block == (1 << (pted->parent->ht_size_log - pted->multiplicity - 1)) and position == pted->multiplicity;
            else return block == -1;
        }
        void advance(bool bk) override {
            if(bk) { position++; if(position == pted->multiplicity) { position = 0; block++; } }
            else { position--; if(position == -1) { position = pted->multiplicity-1; block--; } }
        }
        Leaf<T, b>* down(bool bk) override {
            return pted->parent->ht[2*block*pted->multiplicity + position].createStart(this, bk);
        }
    };

    void init(int mult, Y_Hashtable<T>* par) { multiplicity = mult; parent = par; }
    bool empty() const override {
        return parent == nullptr or parent->partlyEmpty(multiplicity);
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
            for(int blk = 0; blk < (1 << (parent->ht_size_log - multiplicity - 1)); ++blk)
                for (int i = 1; i != multiplicity; ++i)
                    if (!parent->ht[2*blk*multiplicity + i].empty()) {
                        return parent->ht[2 * blk * (multiplicity - 1) + i].createStart(
                                new myiterator<false>(this, iter), bk);
                    }
        } else {
            for (int blk = (1 << (parent->ht_size_log - multiplicity - 1)) - 1; blk != -1; --blk)
                for (unsigned i = multiplicity+1; i != 0; --i)
                    if (!parent->ht[2*blk*multiplicity + i - 1].empty()) {
                        return parent->ht[2*blk*multiplicity + i - 1].createStart(
                                new myiterator<false>(this, iter), bk);
                    }
        }
        return nullptr;
    }
    ConcreteLeaf<T, true>* createStart(inner_iterator<T, true>* iter, bool bk) const override {
        if(bk) {
            for(int blk = 0; blk < (1 << (parent->ht_size_log - multiplicity - 1)); ++blk)
                for (int i = 1; i != multiplicity; ++i)
                    if (!parent->ht[2*blk*multiplicity + i].empty()) {
                        return parent->ht[2 * blk * (multiplicity - 1) + i].createStart(
                                new myiterator<true>(this, iter), bk);
                    }
        } else {
            for (int blk = (1 << (parent->ht_size_log - multiplicity - 1))-1; blk != -1; --blk)
                for (int i = multiplicity+1; i != 0; --i)
                    if (!parent->ht[2*blk*multiplicity + i - 1].empty()) {
                        return parent->ht[2*blk*multiplicity + i - 1].createStart(
                                new myiterator<true>(this, iter), bk);
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

    void restate() {
        parent->refold(multiplicity);
    }
};

#endif //OLYMPUS_STATE_N_H
