#ifndef OLYMPUS_PERMANENT_TN_H
#define OLYMPUS_PERMANENT_TN_H

#include "abstract_n.h"
#include "gameplay/permanents/4permanents.h"
class Player;

class PermanentN {
public:
    virtual ~PermanentN() = default;
    virtual Permanent* getPermanent() = 0;
    virtual const Permanent* getPermanent() const = 0;
};

//the base class is the PermanentTN. It contains (and owns) a permanent.
template <typename T>
class PermanentTN: public Yggdrasil<T>, public PermanentN {
    T describedObject;
    std::list<Yggdrasil_base*> parents;
public:
    PermanentTN(std::unique_ptr<Card> source, Player* pl): describedObject(std::move(source), pl) {}
    bool empty() const override { return false; }
    iterator<T, false> begin() override { return { new ConcreteLeaf<T, false>(this, nullptr) }; }
    iterator<T, true> cbegin() const override { return { new ConcreteLeaf<T, true>(this, nullptr) }; }

    ConcreteLeaf<T, false>* createStart(inner_iterator<T, false>* parent, bool) override {
        return new ConcreteLeaf<T, false>(this, parent);
    }
    ConcreteLeaf<T, true>* createStart(inner_iterator<T, true>* parent, bool) const override {
        return new ConcreteLeaf<T, true>(this, parent);
    }
    Leaf<Permanent, false>* pcreateStart(inner_iterator<Permanent, false>* parent, bool) override {
        return new AdapterLeaf<T, false>( new ConcreteLeaf<T, false>(this, nullptr), parent);
    }
    Leaf<Permanent, true>* pcreateStart(inner_iterator<Permanent, true>* parent, bool) const override {
        return new AdapterLeaf<T, true>( new ConcreteLeaf<T, true>(this, nullptr), parent);
    }
    iterator<Permanent, false> pbegin() override {
        return { new AdapterLeaf<T, false>( new ConcreteLeaf<T, false>(this, nullptr), nullptr) };
    }
    iterator<Permanent, true> cpbegin() const override {
        return { new AdapterLeaf<T, true>(new ConcreteLeaf<T, true>(this, nullptr), nullptr) };
    }
    Permanent* getPermanent() override { return &describedObject; }
    const Permanent* getPermanent() const override { return &describedObject; }
    T* getObject() { return &describedObject; }
    const T* getObject() const { return &describedObject; }

    friend class Leaf<T, true>; friend class Leaf<T, false>;
};

#endif //OLYMPUS_PERMANENT_TN_H
