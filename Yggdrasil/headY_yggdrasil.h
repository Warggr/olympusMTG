#ifndef OLYMPUS_HEADER_Y_YGGDRASIL
#define OLYMPUS_HEADER_Y_YGGDRASIL

#include "../include/.olympus_main.h"
#include "../HFiles/headI_identifiers.h"
#include <list>

template <typename T, bool iconst> class iterator;
template <typename T> class PermanentTN;

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
    template<typename X> iterator<X, false> xbegin();
    template<typename X> iterator<X, true> cxbegin() const;
    template<typename X, bool iconst> iterator<X, iconst> xend() const;
    void add_child(PermanentTN<T>* a);
    void construct_pure_child(Card* crd, Player* pl);
    bool try_to_construct(Card* to_add, Identifier casted_id, Player* pl);
    virtual void state_out(PermanentTN<T>* perm) = 0;
    bool empty() const {return (children.empty() && pure_children_first == 0); };

    iterator<T, false> begin();
    iterator<T, true> cbegin() const;
    iterator<Permanent, false> pbegin();
    iterator<Permanent, true> cpbegin() const;
    iterator<T, false> end() const;
    iterator<T, true> cend() const;
    iterator<Permanent, false> pend() const;
    iterator<Permanent, true> cpend() const;
};

template <typename T> class StatedCollectionTN;

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

#endif //OLYMPUS_HEADER_Y_YGGDRASIL