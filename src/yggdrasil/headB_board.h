#ifndef OLYMPUS_HEADER_5_YGGDRASIL_H
#define OLYMPUS_HEADER_5_YGGDRASIL_H

#include "headY_yggdrasil.h"

template<typename T, bool iconst>
class iterator{
    using BasicObj = typename std::conditional<iconst, const T, T>::type;
    using BasicCont = typename std::conditional<iconst, const PermanentTN<T>, PermanentTN<T>>::type;

    BasicCont* pointed;
    bool is_primary;
public:
    iterator(): pointed(0), is_primary(false){};
    iterator(BasicCont* pted, bool primary): pointed(pted), is_primary(primary){};
    bool get_primary(){return is_primary; }
    //void set_primary(bool prim){is_primary = prim; };
    iterator operator++(){
        pointed = pointed->next;
        return *this;
    }
    iterator operator--(){
        pointed = pointed->prev;
        return *this;
    }
    iterator operator++(int){ return this->operator++(); }
    iterator operator--(int){ return this->operator--(); }
    BasicObj* operator->(){
        return &(this->operator*());
    }
    BasicObj& operator*(){
        return pointed->describedObject;
    }
    bool operator==(const iterator& other) const {
        return (other.pointed == pointed);
    }
    bool operator!=(const iterator& other) const {
        return !(other == *this);
    }
    BasicCont* get_pointed() const {return pointed; };
};

template<bool iconst>
class iterator<Permanent, iconst>{
    using BasicObj = typename std::conditional<iconst, const Permanent, Permanent>::type;
    using BasicCont = typename std::conditional<iconst, const APPermanentN, APPermanentN>::type;

    BasicCont* pointed;
    bool is_primary;
public:
    iterator(): pointed(0), is_primary(false){};
    iterator(BasicCont* pted, bool primary): pointed(pted), is_primary(primary){};
    bool get_primary(){return is_primary; }
    //void set_primary(bool prim){is_primary = prim; };
    iterator operator++(){
        pointed = pointed->pnext;
        return *this;
    }
    iterator operator--(){
        pointed = pointed->pprev;
        return *this;
    }
    iterator operator++(int){ return this->operator++(); }
    iterator operator--(int){ return this->operator--(); }
    BasicObj* operator->(){
        return &(this->operator*());  
    }
    BasicObj& operator*(){
        return pointed->get_describedObject((BasicObj*) nullptr);
    }
    bool operator==(const iterator& other) const {
        return (other.pointed == pointed);
    }
    bool operator!=(const iterator& other) const {
        return !(other == *this);
    }
    BasicCont* get_pointed() const {return pointed; };
};

class APPermanentN{
public:
    APPermanentN* pprev; //this is mainly for convenience, and also probably redundant,
    APPermanentN* pnext;

    APPermanentN(APPermanentN* p, APPermanentN* n): pprev(p), pnext(n) {
        if(pnext) pnext->pprev = this;
    }
    virtual void single_out() = 0; //removes object from its siblings
    virtual void obliterate() = 0; //unconnects from siblings & parents, signal its death to its parents (TODO) and deletes itself
    virtual void unstate() = 0; //changes state
    virtual const Permanent& get_describedObject(const Permanent* signature_compl) const = 0;
    virtual Permanent& get_describedObject(Permanent* signature_compl) = 0;
};

template <typename T>
class PermanentTN: public APPermanentN, public AbstractN { //the base class is the PermanentTN. It contains a permanent.
    T describedObject;
    CollectionTN<T>* primary_parent;
    std::list<CollectionTN<T>*> parents;
public:
    PermanentTN* prev; //Permanents are double-linked with each other in order to achieve a fast scrolling through all perms
    PermanentTN* next;

    PermanentTN(Card* source, Player* pl, PermanentTN* n, CollectionTN<T>* par):
        APPermanentN(0, n), describedObject(source, pl), primary_parent(par), prev(0), next(n){
            if(next) next->prev = this;
        }
    PermanentTN(Card* source, Player* pl, PermanentTN* p, PermanentTN* n, CollectionTN<T>* par):
        APPermanentN(p, n), describedObject(source, pl), primary_parent(par), prev(0), next(n) {
            if (next) next->prev = this;
        }
    virtual ~PermanentTN() = default;

    void unstate(){single_out(); this->primary_parent->state_out(this); }; //changes state
    void add(T* per);
    void single_out(); //removes object from its siblings
    void obliterate(){single_out(); delete this; }; //unconnects from siblings & parents, signal its death to its parents (TODO) and deletes itself

    template<typename X> iterator<X, false> begin(){iterator<X, false> ret(this, true); return ret; };
    template<typename X> iterator<X, true> cbegin() const {iterator<X, true> ret(this, true); return ret; };
    template<typename X, bool iconst> iterator<X, iconst> end() const {iterator<X, iconst> ret(next, true); return ret; };
    const Permanent& get_describedObject(const Permanent* signature_compl) const {return describedObject; };
    Permanent& get_describedObject(Permanent* signature_compl) {return describedObject; };

    template<typename X, bool iconst> friend class iterator;
};

#include "archetypes.cpp"

#endif //OLYMPUS_HEADER_5_YGGDRASIL_H