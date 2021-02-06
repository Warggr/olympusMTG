#ifndef OLYMPUS_HEADER_5_YGGDRASIL_H
#define OLYMPUS_HEADER_5_YGGDRASIL_H

#include "../include/.olympus_main.h"
#include "../HFiles/head7_identifiers.h"
#include <list>

class AbstractN{ //TODO: give triggers (different number of triggers depending on the type)
protected:
    struct ListNode{
        StaticAb* modif;
        ListNode* prev;
        ListNode* next;
        ListNode(StaticAb* st, ListNode* nxt): modif(st), prev(0), next(nxt) {};
    } *all_modifs {0};
    std::list<AbstractN*> parents;
public:
    virtual ~AbstractN() = default;
    ListNode* add_modif(StaticAb* staticAb){ all_modifs = new ListNode(staticAb, all_modifs); return all_modifs; };
    void remove_modif(ListNode* ptr);

    virtual typediterator<Permanent> pbegin() = 0;
    virtual typediterator<Permanent> pend() const = 0;
    virtual c_iterator<Permanent> cpbegin() const = 0;
    virtual c_iterator<Permanent> cpend() const = 0;
};

class AbstractPermanentN{
public:
	virtual AbstractPermanentN* get_pprev() const = 0;
	virtual AbstractPermanentN* get_pnext() const = 0;
	virtual Permanent* get_perm() const = 0;
};

template <typename T>
class PermanentTN: public AbstractPermanentN, public AbstractN { //the base class is the PermanentTN. It contains an (owning) pointer to a permanent.
    T describedObject;
    CollectionTN<T>* primary_parent;
    std::list<CollectionTN<T>*> parents;
public:
    PermanentTN<T>* prev; //Permanents are double-linked with each other in order to achieve a fast scrolling through all perms
    PermanentTN<T>* next;

    PermanentTN(Card* source, Player* pl, PermanentTN* n, AbstractN* par):
        AbstractN(par), describedObject(source, pl), prev(0), next(n) {
            if(next) next->prev = this;
        }
    PermanentTN(Card* source, Player* pl, PermanentTN* p, PermanentTN* n, CollectionTN<T>* par):
        describedObject(source, pl), primary_parent(par), prev(p), next(n) {
            if (next) next->prev = this;
        }
    virtual ~PermanentTN() = default;
    AbstractPermanentN* get_pprev() const {return prev; };
    AbstractPermanentN* get_pnext() const {return next; };
    PermanentTN<T>* get_prev() const {return prev; };
    PermanentTN<T>* get_next() const {return next; };
    Permanent* get_perm() const {return (Permanent*) &describedObject; };
    void unstate(){single_out(); this->primary_parent->state_out(this); }; //changes state
    void add(T* per);
    typediterator<T> begin();
    typediterator<T> end() const;
    typediterator<Permanent> pbegin();
    typediterator<Permanent> pend() const;
    c_iterator<T> cbegin() const;
    c_iterator<T> cend() const;
    c_iterator<Permanent> cpbegin() const;
    c_iterator<Permanent> cpend() const;
    void single_out(); //removes object from its siblings
    void obliterate(){single_out(); delete this; }; //unconnects from siblings & parents, signal its death to its parents (TODO) and deletes itself

    friend class typediterator<T>;
    friend class typediterator<Permanent>;
    friend class c_iterator<T>;
    friend class c_iterator<Permanent>;
};

template <typename T>
class CollectionTN: public AbstractN{
protected:
	Identifier min_chars;
    Identifier min_requ;
    AbstractN* primary_parent;
    std::list<AbstractN*> parents;
    std::list<CollectionTN<T>*> children;
    bool is_primary; //true when all of its children are adjacent in memory
public:
    PermanentTN<T>* pure_children_first;
    PermanentTN<T>* pure_children_last;

    CollectionTN(AbstractN* parent): primary_parent(parent), pure_children_first(0), pure_children_last(0) {};
    CollectionTN(Identifier chars, Identifier requs, AbstractN* parent): min_chars(chars), min_requ(requs), primary_parent(parent), pure_children_first(0), pure_children_last(0) {};
    virtual ~CollectionTN() = default;
    void ragnarok_collectiontn();
    void full_ragnarok_boardzone();
    typediterator<T> begin();
    typediterator<T> end() const;
    typediterator<Permanent> pbegin();
    typediterator<Permanent> pend() const;
    c_iterator<T> cbegin() const;
    c_iterator<T> cend() const;
    c_iterator<Permanent> cpbegin() const;
    c_iterator<Permanent> cpend() const;
    void add_child(PermanentTN<T>* a);
    void construct_pure_child(Card* crd, Player* pl);
    bool try_to_construct(Card* to_add, Identifier casted_id, Player* pl);
    virtual void state_out(PermanentTN<T>* perm) = 0;
    bool empty() const {return (children.empty() && pure_children_first == 0); };
};

template <typename T>
class DefaultCollectionTN: public CollectionTN<T>{
    StatedCollectionTN<T>* states;
public:
	DefaultCollectionTN(BoardN* parent): CollectionTN<T>(T::def_identifier, rid_perm_type, parent){};
    DefaultCollectionTN(Card* crd, Player* pl, AbstractN* parent, bool primary): CollectionTN<T>(parent)
		{this->construct_pure_child(crd, pl); this->is_primary = true; };
	DefaultCollectionTN(Identifier chars, Identifier requs, AbstractN* par): CollectionTN<T>(par, chars, requs) {};
	~DefaultCollectionTN(){if(states) delete states; };
    StatedCollectionTN<T>* def_new_state();
	void restate(); //TODO merge only certain states, or even check for each permanent whether it wants to be merged
    void state_out(PermanentTN<T>* child); //TODO make it depend on the actual state we put this in
};

template <typename T>
class StatedCollectionTN: public CollectionTN<T>{
	DefaultCollectionTN<T>* default_state;
public:
    StatedCollectionTN* next_state;

	StatedCollectionTN(DefaultCollectionTN<T>* original_state, AbstractN* parent, StatedCollectionTN* next):
	    CollectionTN<T>(parent), default_state(original_state), next_state(next) {};
	~StatedCollectionTN(){if(next_state) delete next_state; }
	void state_out(PermanentTN<T>* perm){}; //already stated out

	friend class CollectionTN<T>;
};

template<typename T>
class typediterator{
    PermanentTN<T>* pointed;
    bool is_primary;
public:
    typediterator(): pointed(0), is_primary(false){};
    typediterator(PermanentTN<T>* pted, bool primary): pointed(pted), is_primary(primary){};
    void set_primary(bool prim){is_primary = prim; };
    typediterator operator++(){
        pointed = pointed->next;
        return *this;
    }
    typediterator operator--(){
        pointed = pointed->prev;
        return *this;
    }
    typediterator operator++(int){
        return this->operator++();
    }
    typediterator operator--(int){
        return this->operator--();
    }
    T* operator->(){
        return &(pointed->describedObject);
    }
    T& operator*(){
        return pointed->describedObject;
    }
    bool operator==(const typediterator& other) const {
        return (other.pointed == pointed);
    }
    bool operator!=(const typediterator& other) const {
        return !(other == *this);
    }
    PermanentTN<T>* get_pointed() const {return pointed; };
};

template<typename T>
class c_iterator{
    const PermanentTN<T>* pointed;
    bool is_primary;
public:
    c_iterator(): pointed(0), is_primary(false){};
    c_iterator(const PermanentTN<T>* pted, bool primary): pointed(pted), is_primary(primary){};
    c_iterator operator++(){
        pointed = pointed->next;
        return *this;
    }
    c_iterator operator--(){
        pointed = pointed->prev;
        return *this;
    }
    c_iterator operator++(int){
        return this->operator++();
    }
    c_iterator operator--(int){
        return this->operator--();
    }
    const T* operator->(){
        return &(pointed->describedObject);
    }
    const T& operator*(){
        return pointed->describedObject;
    }
    bool operator==(const c_iterator& other) const {
        return (other.pointed == pointed);
    }
    bool operator!=(const c_iterator& other) const {
        return !(other == *this);
    }
    const PermanentTN<T>* get_pointed() const {return pointed; };
};

template <>
class typediterator<Permanent>{
    AbstractPermanentN* pointed;
    bool is_primary;
public:
    typediterator(): pointed(0), is_primary(false){};
    typediterator(AbstractPermanentN* pted, bool primary): pointed(pted), is_primary(primary){};
    typediterator operator++(){
        if(is_primary) pointed = pointed->get_pnext();
        return *this;
    }
    typediterator operator--(){
        if(is_primary) pointed = pointed->get_pprev();
        return *this;
    }
    Permanent* operator->(){
        return pointed->get_perm();
    }
    Permanent& operator*(){
        return *(this->operator->());
    }
    typediterator operator++(int){
        return this->operator++();
    }
    typediterator operator--(int){
        return this->operator--();
    }
    bool operator==(const typediterator& other) const {
        return (other.pointed == pointed);
    }
    bool operator!=(const typediterator& other) const {
        return !(other == *this);
    }
};

template <>
class c_iterator<Permanent>{
    const AbstractPermanentN* pointed;
    bool is_primary;
public:
    c_iterator(): pointed(0), is_primary(false){};
    c_iterator(const AbstractPermanentN* pted, bool primary): pointed(pted), is_primary(primary){};
    c_iterator operator++(){
        if(is_primary) pointed = pointed->get_pnext();
        return *this;
    }
    c_iterator operator--(){
        if(is_primary) pointed = pointed->get_pprev();
        return *this;
    }
    const Permanent* operator->(){
        return pointed->get_perm();
    }
    const Permanent& operator*(){
        return *(this->operator->());
    }
    c_iterator operator++(int){
        return this->operator++();
    }
    c_iterator operator--(int){
        return this->operator--();
    }
    bool operator==(const c_iterator& other) const {
        return (other.pointed == pointed);
    }
    bool operator!=(const c_iterator& other) const {
        return !(other == *this);
    }
};

template <typename T> typediterator<T> CollectionTN<T>::begin(){
    if(!children.empty()) return children.front()->begin();
    else if(pure_children_first) return pure_children_first->begin();
    else return typediterator<T>();
}
template <typename T> typediterator<T> CollectionTN<T>::end() const {
    if(pure_children_last) return pure_children_last->end();
    else if(!children.empty()) return children.back()->end(); 
    else return typediterator<T>();
}
template <typename T> typediterator<Permanent> CollectionTN<T>::pbegin(){
    if(!children.empty()) return children.front()->pbegin();
    else if(pure_children_first) return pure_children_first->pbegin();
    else return typediterator<Permanent>();
}
template <typename T> typediterator<Permanent> CollectionTN<T>::pend() const {
    if(pure_children_last) return pure_children_last->pend();
    else if(!children.empty()) return children.back()->pend(); 
    else return typediterator<Permanent>();
}
template <typename T> c_iterator<T> CollectionTN<T>::cbegin() const {
    if(!children.empty()) return children.front()->cbegin();
    else if(pure_children_first) return pure_children_first->cbegin();
    else return c_iterator<T>();
}
template <typename T> c_iterator<T> CollectionTN<T>::cend() const {
    if(pure_children_last) return pure_children_last->cend();
    else if(!children.empty()) return children.back()->cend(); 
    else return c_iterator<T>();
}
template <typename T> c_iterator<Permanent> CollectionTN<T>::cpbegin() const {
    if(!children.empty()) return children.front()->cpbegin();
    else if(pure_children_first) return pure_children_first->cpbegin();
    else return c_iterator<Permanent>();
}
template <typename T> c_iterator<Permanent> CollectionTN<T>::cpend() const {
    if(pure_children_last) return pure_children_last->cpend();
    else if(!children.empty()) return children.back()->cpend(); 
    else return c_iterator<Permanent>();
}
template <typename T> typediterator<T> PermanentTN<T>::begin(){
    return typediterator<T>(this, true);
}
template <typename T> typediterator<T> PermanentTN<T>::end() const {
    return typediterator<T>(next, true);
}
template <typename T> typediterator<Permanent> PermanentTN<T>::pbegin(){
    return typediterator<Permanent>(this, true);
}
template <typename T> typediterator<Permanent> PermanentTN<T>::pend() const {
    return typediterator<Permanent>(next, true);
}
template <typename T> c_iterator<T> PermanentTN<T>::cbegin() const {
    return c_iterator<T>(this, true);
}
template <typename T> c_iterator<T> PermanentTN<T>::cend() const {
    return c_iterator<T>(next, true);
}
template <typename T> c_iterator<Permanent> PermanentTN<T>::cpbegin() const {
    return c_iterator<Permanent>(this, true);
}
template <typename T> c_iterator<Permanent> PermanentTN<T>::cpend() const {
    return c_iterator<Permanent>(next, true);
}

#include "archetypes.cpp"

#endif //OLYMPUS_HEADER_5_YGGDRASIL_H