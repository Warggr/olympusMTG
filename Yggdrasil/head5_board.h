#ifndef OLYMPUS_HEADER_5_YGGDRASIL_H
#define OLYMPUS_HEADER_5_YGGDRASIL_H

#define MANUAL_IMPORT_OF_OLYMPUS_HEADERS
#include "../include/.olympus_main.h"
#include "../include/4permanents.h"
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

class BoardN: public AbstractN {
public:
    DefaultCollectionTN<Land> mylands;
    DefaultCollectionTN<Artifact> myartos;
    DefaultCollectionTN<Planeswalker> mysuperfriends;
    DefaultCollectionTN<Creature> mycreas;
    StatedCollectionTN<Creature>* myattackers {0};

    BoardN(): mylands(this), myartos(this), mysuperfriends(this), mycreas(this), myattackers(0){};
    ~BoardN();

    void insert(Card* to_add, Player* pl);
    typediterator<Permanent> pbegin();
    typediterator<Permanent> pend() const;
    c_iterator<Permanent> cpbegin() const;
    c_iterator<Permanent> cpend() const;
};

#include "head6_iterators.h"
#include "archetypes.cpp"

#endif //OLYMPUS_HEADER_5_YGGDRASIL_H