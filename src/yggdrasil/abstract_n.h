#ifndef OLYMPUS_YGGDRASIL_1_H
#define OLYMPUS_YGGDRASIL_1_H

#include "oracles/classes/3statics.h"
#include "headB_board.h"
#include "headI_identifiers.h"
#include <list>

class Yggdrasil_base {
protected:
    ModifListNode* all_modifs {nullptr};
public:
    virtual ~Yggdrasil_base() = default;
    ModifListNode* add_modif(StaticAb_H* staticAb){ all_modifs = new ModifListNode(staticAb, all_modifs); return all_modifs; };
    void remove_modif(ModifListNode* ptr) {
        if(ptr->prev) ptr->prev->next = ptr->next;
        else all_modifs = ptr->next;
        if(ptr->next) ptr->next->prev = ptr->prev;
    }

    virtual iterator<Permanent, false> pbegin() = 0;
    virtual iterator<Permanent, true> cpbegin() const = 0;
    virtual Leaf<Permanent, false>* pcreateStart(inner_iterator<Permanent, false>* parent) = 0;
    virtual Leaf<Permanent, true>* pcreateStart(inner_iterator<Permanent, true>* parent) const = 0;
    constexpr nullptr_t pend() const { return nullptr; }
    constexpr nullptr_t cpend() const { return nullptr; }

    virtual bool empty() const = 0;
};

template<typename T>
class Yggdrasil_N : public Yggdrasil_base {
public:
    virtual ~Yggdrasil_N() = default;
    virtual iterator<T, false> begin() = 0;
    virtual iterator<T, true> cbegin() const = 0;
    virtual ConcreteLeaf<T, false>* createStart(inner_iterator<T, false>* parent) = 0;
    virtual ConcreteLeaf<T, true>* createStart(inner_iterator<T, true>* parent) const = 0;
    constexpr nullptr_t end() const { return nullptr; }
    constexpr nullptr_t cend() const { return nullptr; }
};

template<typename T>
class Yggdrasil: public Yggdrasil_N<T> {
public:
    virtual ~Yggdrasil() = default;
};

template<>
class Yggdrasil<Permanent>: public Yggdrasil_N<Permanent> {
public:
    virtual ~Yggdrasil() = default;

    iterator<Permanent, false> pbegin() override { return begin(); }
    iterator<Permanent, true> cpbegin() const override { return cbegin(); }
    Leaf<Permanent, false>* pcreateStart(inner_iterator<Permanent, false>* parent) override {
        return createStart(parent);
    }
    Leaf<Permanent, true>* pcreateStart(inner_iterator<Permanent, true>* parent) const override {
        return createStart(parent);
    }
};

#endif //OLYMPUS_YGGDRASIL_1_H
