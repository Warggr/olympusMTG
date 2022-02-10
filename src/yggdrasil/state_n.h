#ifndef OLYMPUS_STATE_N_H
#define OLYMPUS_STATE_N_H

#include "abstract_n.h"
#include "hashtable_tn.h"

/** Points towards a subset of children, which it does not really own but which are contained by an Y_Hashtable. */
template<typename T>
class StateTN: public Yggdrasil<T> {
    Y_Hashtable<T>* parent;
    int multiplicity; //log-size of a pattern.
    //E.g multiplicity 0: 111111... 1:010101... 2: 00110011... 3: 000011110...
    //block-size is 1 << (multiplicity - 1), block-number is 1 << (ht_size_log - multiplicity)
    inline int nbBlocks() const { return parent->nbBlocks(multiplicity); }
    inline int blockSize() const { return BLOCKSIZE; }
public:
    template<bool b> class myiterator: public inner_iterator<T, b> {
        isitconst(StateTN<T>, b)* pted;
        int block, position;
    public:
        myiterator(isitconst(StateTN<T>, b)* pted, inner_iterator<T, b>* parent):
            inner_iterator<T, b>(parent), pted(pted), block(0), position(0) {};
        bool isEnd(bool bk) const override {
            if(bk) return block == pted->nbBlocks();
            else return block == -1;
        }
        void advance(bool bk) override {
            if(bk) { position++; if(position == pted->blockSize()) { position = 0; block++; } }
            else { position--; if(position == -1) { position = pted->blockSize()-1; block--; } }
        }
        Leaf<T, b>* down(bool bk) override {
            return pted->parent->getChild(block, position, pted->multiplicity).createStart(this, bk);
        }
        void present(uint indent, logging::record_ostream& strm) const override {
            if(iterator_treenode<T, b>::parent) iterator_treenode<T, b>::parent->present(indent + 1, strm);
            for(uint i = 0; i<indent; i++) strm << '>';
            strm << "StateTN @" << pted << " at block " << block << ", position " << position << '\n';
        }
    };

    void init(int mult, Y_Hashtable<T>* par) { multiplicity = mult; parent = par; }
    bool empty() const override {
        return parent == nullptr or parent->partlyEmpty(multiplicity);
    }
    unsigned int size() const override {
        return parent == nullptr ? 0 : parent->partialSize(multiplicity);
    }
    iterator<T, false> begin() override { return { createStart(nullptr, true) }; }
    iterator<T, true> cbegin() const override { return { createStart(nullptr, true) }; }
    iterator<Permanent, false> pbegin() override {
        return { AdapterLeaf<T, false>::create( createStart(nullptr, true), nullptr) };
    }
    iterator<Permanent, true> cpbegin() const override {
        return { AdapterLeaf<T, true>::create( createStart(nullptr, true), nullptr) };
    }
    ConcreteLeaf<T, false>* createStart(inner_iterator<T, false>* iter, bool bk) override {
        if(!parent) return nullptr;
        if(bk) {
            for(int blk = 0; blk < nbBlocks(); ++blk)
                for (int i = 0; i != blockSize(); ++i)
                    if (!parent->getChild(blk, i, multiplicity).empty()) {
                        return parent->getChild(blk, i, multiplicity).createStart(
                                new myiterator<false>(this, iter), bk);
                    }
        } else {
            for (int blk = nbBlocks() - 1; blk != -1; --blk)
                for (int i = blockSize() - 1; i != -1; --i)
                    if (!parent->getChild(blk, i, multiplicity).empty()) {
                        return parent->getChild(blk, i, multiplicity).createStart(
                                new myiterator<false>(this, iter), bk);
                    }
        }
        return nullptr;
    }
    ConcreteLeaf<T, true>* createStart(inner_iterator<T, true>* iter, bool bk) const override {
        if(!parent) return nullptr;
        if(bk) {
            for(int blk = 0; blk < nbBlocks(); ++blk)
                for (int i = 0; i != blockSize() ; ++i)
                    if (!parent->getChild(blk, i, multiplicity).empty()) {
                        return parent->getChild(blk, i, multiplicity).createStart(
                                new myiterator<true>(this, iter), bk);
                    }
        } else {
            for (int blk = nbBlocks() - 1; blk != -1; --blk)
                for (int i = blockSize() - 1; i != 0; --i)
                    if (!parent->getChild(blk, i, multiplicity).empty()) {
                        return parent->getChild(blk, i, multiplicity).createStart(
                                new myiterator<true>(this, iter), bk);
                    }
        }
        return nullptr;
    }
    Leaf<Permanent, false>* pcreateStart(inner_iterator<Permanent, false>* iter, bool bk) override {
        return AdapterLeaf<T, false>::create(createStart(nullptr, bk), iter);
    }
    Leaf<Permanent, true>* pcreateStart(inner_iterator<Permanent, true>* iter, bool bk) const override {
        return AdapterLeaf<T, true>::create(createStart(nullptr, bk), iter);
    }

    void restate() {
        parent->refold(multiplicity);
        parent = nullptr;
    }

    void disp(unsigned int indent, logging::record_ostream& strm) const override {
        for(uint i=0; i<indent; i++) strm << ' ';
        strm << "---State @" << this << " viewing parent " << parent << " at multiplicity " << multiplicity << '\n';
    }
};

#endif //OLYMPUS_STATE_N_H
