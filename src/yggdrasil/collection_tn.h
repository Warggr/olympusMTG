#ifndef OLYMPUS_YGGDRASIL_2_H
#define OLYMPUS_YGGDRASIL_2_H

#include "abstract_n.h"
#include "permanent_tn.h"
#include <list>
#include <algorithm>
#include <iostream>
#include <functional>

template<class T> class Y_Hashtable;
class BoardN; class Player;

template <typename T>
class CollectionTN: public Yggdrasil<T> {
protected:
    Y_Hashtable<T>* parent;
    std::list<PermanentTN<T>> children;
public:
    template<bool iconst>
    class myiterator : public inner_iterator<T, iconst> {
        typename std::conditional<iconst, const CollectionTN<T>, CollectionTN<T>>::type* node;
        typename std::conditional<iconst,
            typename std::list<PermanentTN<T>>::const_iterator,
            typename std::list<PermanentTN<T>>::iterator>::type it;
    public:
        myiterator(isitconst(CollectionTN<T>, iconst)* n, inner_iterator<T, iconst>* in = nullptr):
            inner_iterator<T, iconst>(in), node(n), it(n->children.begin()) {}
        const CollectionTN* getPted() const { return node; }
        void advance(bool bk) override { if(bk) ++it; else --it; }
        bool isEnd(bool bk) const override { return (bk and it == node->children.end()) or (!bk and it == --node->children.begin()); }
        Leaf<T, iconst>* down(bool bk) override {
            return it->createStart(this, bk);
        }
        void present(uint indent, logging::record_ostream& strm) const override {
            if(iterator_treenode<T, iconst>::parent) iterator_treenode<T, iconst>::parent->present(indent + 1, strm);
            for(uint i = 0; i<indent; i++) strm << '>';
            strm << "Collection @" << node << " at pos <somewhere in list> \n";
        }
        friend class CollectionTN;
    };

    explicit CollectionTN(Y_Hashtable<T>* parent = nullptr): parent(parent) {}
    CollectionTN& operator=(CollectionTN&& other) noexcept { parent = other.parent; children = std::move(other.children); return *this; }
    inline void setParent(Y_Hashtable<T>* pr) { parent = pr; }

    void insert(card_ptr origin, Player* pl) {
        children.template emplace_back(move_cardptr(origin), pl);
    }

    bool empty() const override { for(auto& i : children) if(!i.empty()) return false; return true; }
    unsigned int size() const override {
        uint size = 0; for(auto& i : children) size += i.size(); return size;
    }

    void remove(T* object) {
        (void) object; //TODO CRITICAL implement
    };

    void merge(CollectionTN& other) {
        children.splice(children.end(), other.children);
    }

    bool split(std::function<bool(T)> predicate, CollectionTN& other) {
        bool ret = false;
        for(auto iter = children.begin(); iter != children.end(); ){
            if(predicate(*iter)){
                auto i2 = iter;
                i2++;
                other.children.splice(other.children.begin(), *this, iter);
                ret = true;
                iter = i2;
            } else {
                ++iter;
            }
        }
        return ret;
    }

    template<bool iconst>
    iterator<T, iconst> splice(CollectionTN& other, iterator<T, iconst> position) {
//        std::cout << "Splicing me (@" << this << ") inserting from other (@" << &other << ")\n";

        myiterator<iconst>* iter = position.template findFor<CollectionTN<T>>(&other);
//        std::cout << "Found iterator @" << iter << " (node=" << iter->node << ")\n";
        typename std::list<PermanentTN<T>>::iterator it2 = iter->it;
        it2++;

        children.splice(children.begin(), other.children, iter->it);
        iter->it = it2;
        return position;
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
        return children.empty() ? nullptr :
        bk ? children.begin()->createStart(new myiterator<false>(this, iter), true) :
        (--(children.end()))->createStart(new myiterator<false>(this, iter), false);
    }
    ConcreteLeaf<T, true>* createStart(inner_iterator<T, true>* iter, bool bk) const override {
        return children.empty() ? nullptr :
        bk ? children.begin()->createStart(new myiterator<true>(this, iter), true) :
        (--(children.end()))->createStart(new myiterator<true>(this, iter), false);
    }
    Leaf<Permanent, false>* pcreateStart(inner_iterator<Permanent, false>* iter, bool bk) override {
        return AdapterLeaf<T, false>::create(createStart(nullptr, bk), iter);
    }
    Leaf<Permanent, true>* pcreateStart(inner_iterator<Permanent, true>* iter, bool bk) const override {
        return AdapterLeaf<T, true>::create(createStart(nullptr, bk), iter);
    }

    void disp(unsigned int indent, logging::record_ostream& strm) const override {
        for(uint i=0; i<indent; i++) strm << ' ';
        strm << "---Collection @" << this << '\n';
        for(const auto& i : children) i.disp(indent + 1, strm);
    };

#ifdef F_TESTS
    const std::list<PermanentTN<T>>& getChildren() const { return children; }
#endif
};

#endif //OLYMPUS_YGGDRASIL_2_H
