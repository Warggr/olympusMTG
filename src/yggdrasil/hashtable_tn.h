#ifndef OLYMPUS_YGGDRSAIL_H
#define OLYMPUS_YGGDRSAIL_H

#include "abstract_n.h"
#include "collection_tn.h"
#include <functional>
class BoardN; class Player; class Card;
template<typename T> class StateTN;

#define HT_SIZE (1 << ht_size_log)
#define BLOCKSIZE (1 << (multiplicity - 1))
#define PATTERNSIZE (1 << multiplicity)

template<typename T>
class Y_Hashtable : public Yggdrasil<T> {
protected:
    BoardN* parent;
    int ht_size_log;
    CollectionTN<T>* ht;
public:
    inline CollectionTN<T>& getChild(int block, int i, int multiplicity, bool first) const { return ht[ block * PATTERNSIZE + i + (first ? 0 : BLOCKSIZE) ]; }
    inline int nbBlocks(int multiplicity) const { return 1 << (ht_size_log - multiplicity); }

    template<bool b>
    class myiterator : public inner_iterator<T, b> {
        char position;
        isitconst(Y_Hashtable*, b) node;
    public:
        explicit myiterator(isitconst(Y_Hashtable*, b) n, inner_iterator<T, b>* parent = nullptr) :
            inner_iterator<T, b>(parent), position(0), node(n) {};
        void advance(bool bk) override { if(bk) position++; else position--; }
        bool isEnd(bool bk) const override { if(bk) return position >= (1<<node->ht_size_log); else return position == -1; }
        Leaf<T, b>* down(bool bk) override { return node->createStart(this, bk); }
        void present(uint indent, logging::record_ostream& strm) const override {
            if(iterator_treenode<T, b>::parent) iterator_treenode<T, b>::parent->present(indent + 1, strm);
            for(uint i = 0; i<indent; i++) strm << '>';
            strm << "Hashtable @" << node << " at pos " << static_cast<int>(position) << '\n';
        }
    };

    Y_Hashtable(BoardN* parent): parent(parent), ht_size_log(0), ht(new CollectionTN<T>[1])  { ht->setParent(this); }
    bool split(std::function<bool(T)> predicate, int multiplicity) {
        bool ret = false;
        for(int blk = 0; blk < nbBlocks(multiplicity); ++blk)
            for (int i = 0; i < BLOCKSIZE; ++i ) {
                ret = ht[blk * PATTERNSIZE + i].split(predicate, ht[blk * PATTERNSIZE + BLOCKSIZE + i]) || ret;
            }
        return ret;
    }
    void unfold(StateTN<T>* newState) {
        auto* new_ht = new CollectionTN<T>[(1 << (ht_size_log+1))];
        for(int i=0; i<HT_SIZE; i++) {
            new_ht[i] = std::move(ht[i]);
            new_ht[HT_SIZE + i].setParent(this);
        }
        ht_size_log++;
        delete[] ht;
        ht = new_ht;
        newState->init(ht_size_log, this);
    }
    void refold(int multiplicity) {
        //TODO IMPL be able to refold a fold that's not the last one
        auto* new_ht = new CollectionTN<T>[(1 << (ht_size_log-1))];
        for(int blk = 0; blk < nbBlocks(multiplicity); ++blk )
            for (int i = 0; i < BLOCKSIZE; ++i ) {
                new_ht[blk * PATTERNSIZE + i] = std::move(ht[blk * PATTERNSIZE + i]);
                new_ht[blk * PATTERNSIZE + i].merge(ht[blk * PATTERNSIZE + BLOCKSIZE + i]);
            }
        ht_size_log--;
        delete[] ht;
        ht = new_ht;
    }
    inline CollectionTN<T>* firstNonEmpty(int multiplicity, bool first) {
        for(int blk = 0; blk < nbBlocks(multiplicity); ++blk )
            for (int i = 0; i < BLOCKSIZE; ++i )
                if (!getChild(blk, i, multiplicity, first).empty()) return &(getChild(blk, i, multiplicity, first));
        return nullptr;
    }
    inline const CollectionTN<T>* firstNonEmpty(int multiplicity, bool first) const {
        return const_cast<Y_Hashtable<T>*>(this)->firstNonEmpty(multiplicity, first);
    }
    inline bool partlyEmpty(int multiplicity, bool first) const {
        return firstNonEmpty(multiplicity, first) == nullptr;
    }
    inline unsigned int partialSize(int multiplicity, bool first) const {
        unsigned int size = 0;
        for(int blk = 0; blk < nbBlocks(multiplicity); ++blk )
            for (int i = 0; i < BLOCKSIZE; ++i )
                size += getChild(blk, i, multiplicity, first).size();
        return size;
    }
    bool empty() const override {
        for(int i = 0; i < HT_SIZE; ++i )
            if (!ht[i].empty()) return false;
        return true;
    }
    unsigned int size() const override { 
        unsigned int size = 0;
        for(int i = 0; i < HT_SIZE; ++i )
            size += ht[i].size();
        return size;
    }
    iterator<T, false> begin() override { return { createStart(nullptr, true) }; }
    iterator<T, true> cbegin() const override { return { createStart(nullptr, true) }; }
    iterator<Permanent, false> pbegin() override {
        //TODO OPTIMIZE for Permanents, these shouldn't override
        return { AdapterLeaf<T, false>::create( createStart(nullptr, true), nullptr) };
    }
    iterator<Permanent, true> cpbegin() const override {
        return { AdapterLeaf<T, true>::create( createStart(nullptr, true), nullptr) };
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
        return AdapterLeaf<T, false>::create(createStart(nullptr, bk), iter);
    }
    Leaf<Permanent, true>* pcreateStart(inner_iterator<Permanent, true>* iter, bool bk) const override {
        return AdapterLeaf<T, true>::create(createStart(nullptr, bk), iter);
    }

    void insert(card_ptr origin, Player* pl) {
        ht[0].insert(move_cardptr(origin), pl); //TODO FEATURE check whether the permanent fulfills one or more criteria
    }

    void remove(T* object) {
        (void) object; //TODO CRITICAL implement
    };

    void disp(unsigned int indent, logging::record_ostream& strm) const override {
        for(uint i=0; i<indent; i++) strm << ' ';
        strm << "---Hashtable @" << this << " of size " << HT_SIZE << '\n';
        for(int i=0; i<HT_SIZE; i++) ht[i].disp(indent + 1, strm);
    };

#ifdef F_TESTS
    int getSize() const { return ht_size_log; }
    const CollectionTN<T>* getObject(int index) const { return &ht[index]; }
#endif
};

#endif //OLYMPUS_YGGDRSAIL_H
