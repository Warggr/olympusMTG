#ifndef OLYMPUS_YGGDRASIL_1_H
#define OLYMPUS_YGGDRASIL_1_H

#include "classes/3statics.h"
#include "iterators.h"
#include <list>

/** Base class from which all Yggdrasil classes derive. Contains a list of modifs attached to it.
 * Requirement: Children must be iterable.*/
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
    inline iterator<Permanent, true> pbegin() const { return cpbegin(); }
    virtual iterator<Permanent, true> cpbegin() const = 0;
    virtual Leaf<Permanent, false>* pcreateStart(inner_iterator<Permanent, false>* parent, bool bk) = 0;
    virtual Leaf<Permanent, true>* pcreateStart(inner_iterator<Permanent, true>* parent, bool bk) const = 0;
    constexpr iterator<Permanent, false> pend() { return { nullptr }; }
    constexpr iterator<Permanent, true> pend() const { return cpend(); }
    constexpr iterator<Permanent, true> cpend() const { return { nullptr }; }

    virtual bool empty() const = 0;
    virtual unsigned int size() const = 0; //TODO DREAM cache it somewhere
    virtual void disp(unsigned int indent, logging::record_ostream& strm) const = 0;
};

/** Helper class. Do not instantiate, use class Yggdrasil instead. */
template<typename T>
class Yggdrasil_N : public Yggdrasil_base {
public:
    virtual ~Yggdrasil_N() = default;
    virtual iterator<T, false> begin() = 0;
    iterator<T, true> begin() const { return cbegin(); }
    virtual iterator<T, true> cbegin() const = 0;
    virtual ConcreteLeaf<T, false>* createStart(inner_iterator<T, false>* parent, bool bk) = 0;
    virtual ConcreteLeaf<T, true>* createStart(inner_iterator<T, true>* parent, bool bk) const = 0;
    constexpr iterator<T, false> end() { return { nullptr }; }
    constexpr iterator<T, true> end() const { return cend(); }
    constexpr iterator<T, true> cend() const { return { nullptr }; }
};

/** Default Yggdrasil<T> object: has iterators pointing to T */
template<typename T>
class Yggdrasil: public Yggdrasil_N<T> {
public:
    virtual ~Yggdrasil() = default;
};

/**For Yggdrasil<Permanent> specifically, iterating as a Permanent or as a T are the same */
template<>
class Yggdrasil<Permanent>: public Yggdrasil_N<Permanent> {
public:
    virtual ~Yggdrasil() = default;

    iterator<Permanent, false> pbegin() override { return begin(); }
    iterator<Permanent, true> cpbegin() const override { return cbegin(); }
    Leaf<Permanent, false>* pcreateStart(inner_iterator<Permanent, false>* parent, bool bk) override { return createStart(parent, bk); }
    Leaf<Permanent, true>* pcreateStart(inner_iterator<Permanent, true>* parent, bool bk) const override { return createStart(parent, bk); }
};

#endif //OLYMPUS_YGGDRASIL_1_H
