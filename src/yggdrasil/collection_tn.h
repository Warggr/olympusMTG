#ifndef OLYMPUS_YGGDRASIL_2_H
#define OLYMPUS_YGGDRASIL_2_H

#include "abstract_n.h"
#include "permanent_tn.h"

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
        void advance(bool bk) override { if(bk) ++it; else --it; }
        bool isEnd(bool bk) const override { return (bk and it == node->children.end()) or (!bk and it == --node->children.begin()); }
        Leaf<T, iconst>* down(bool bk) override {
            return it->createStart(this, bk);
        }
    };

    explicit CollectionTN(Y_Hashtable<T>* parent = nullptr): parent(parent) {}
    CollectionTN& operator=(CollectionTN&& other) noexcept { parent = other.parent; children = std::move(other.children); return *this; }
    inline void setParent(Y_Hashtable<T>* pr) { parent = pr; }

    void insert(std::unique_ptr<Card> origin, Player* pl) {
        children.template emplace_back(std::move(origin), pl);
    }

    bool empty() const override { for(auto& i : children) if(!i.empty()) return false; return true; }

    void remove(T* object) {
        (void) object; //TODO implement
    };

    void merge(CollectionTN& other) {
        children.splice(children.end(), other.children);
    }

    /*void slice_in_two(Criterion crit, CollectionTN<T>* fulfilled, CollectionTN<T>* not_fulfilled) {
        for(auto i = children.begin(); i != children.end(); ++i) {
            if(i->fulfills(crit)) {
                not_fulfilled->objs.splice(not_fulfilled->objs.begin(), children, i);
            }
        }
        fulfilled->objs = std::move(children);
    }*/
    iterator<T, false> begin() override { return { createStart(nullptr, true) }; }
    iterator<T, true> cbegin() const override { return { createStart(nullptr, true) }; }
    iterator<Permanent, false> pbegin() override {
        return { new AdapterLeaf<T, false>( createStart(nullptr, true), nullptr) };
    }
    iterator<Permanent, true> cpbegin() const override {
        return { new AdapterLeaf<T, true>( createStart(nullptr, true), nullptr) };
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
        return new AdapterLeaf<T, false>(createStart(nullptr, bk), iter);
    }
    Leaf<Permanent, true>* pcreateStart(inner_iterator<Permanent, true>* iter, bool bk) const override {
        return new AdapterLeaf<T, true>(createStart(nullptr, bk), iter);
    }
#ifdef F_TESTS
    const std::list<PermanentTN<T>>& getChildren() const { return children; }
#endif
};

#endif //OLYMPUS_YGGDRASIL_2_H