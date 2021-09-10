#ifndef OLYMPUS_YGGDRASIL_2_H
#define OLYMPUS_YGGDRASIL_2_H

#include "abstract_n.h"
#include "permanent_tn.h"

class BoardN; class Player;

class CollectionA {
public:
    bool isEnd();
};

template <typename T>
class CollectionTN: public Yggdrasil<T> {
protected:
    BoardN* parent;
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
        void advance() override { it++; }
        bool isEnd() const override { return it == node->children.end(); }
        Leaf<T, iconst>* down() override {
            return it->createStart(this);
        }
    };

    explicit CollectionTN(BoardN* parent): parent(parent) {}

    void insert(std::unique_ptr<Card> origin, Player* pl) {
        (void) origin; (void) pl;
        //TODO
    }

    bool empty() const override { for(auto& i : children) if(!i.empty()) return false; return true; }

    void remove(T* object) {
        (void) object; //TODO implement
    };

    void slice_in_two(/*Criterion crit,*/ CollectionTN<T>* fulfilled, CollectionTN<T>* not_fulfilled) {
        /*for(auto i = children.begin(); i != children.end(); ++i) {
            if(i->fulfills(crit)) {
                not_fulfilled->objs.splice(not_fulfilled->objs.begin(), children, i);
            }
        }*/
        fulfilled->objs = std::move(children);
    }
    iterator<T, false> begin() override { return { createStart(nullptr) }; }
    iterator<T, true> cbegin() const override { return { createStart(nullptr) }; }
    iterator<Permanent, false> pbegin() override {
        return { new AdapterLeaf<T, false>( createStart(nullptr), nullptr) };
    }
    iterator<Permanent, true> cpbegin() const override {
        return { new AdapterLeaf<T, true>( createStart(nullptr), nullptr) };
    }
    ConcreteLeaf<T, false>* createStart(inner_iterator<T, false>* iter) override {
        return children.begin()->createStart(new myiterator<false>(this, iter));
    }
    ConcreteLeaf<T, true>* createStart(inner_iterator<T, true>* iter) const override {
        return children.begin()->createStart(new myiterator<true>(this, iter));
    }
    Leaf<Permanent, false>* pcreateStart(inner_iterator<Permanent, false>* iter) override {
        return new AdapterLeaf<T, false>(createStart(nullptr), iter);
    }
    Leaf<Permanent, true>* pcreateStart(inner_iterator<Permanent, true>* iter) const override {
        return new AdapterLeaf<T, true>(createStart(nullptr), iter);
    }
};

#endif //OLYMPUS_YGGDRASIL_2_H
