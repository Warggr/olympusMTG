#ifndef OLYMPUS_YGGDRASIL_PART_3
#define OLYMPUS_YGGDRASIL_PART_3

#include "head5_board.h"
#include "head6_iterators.h"

template <typename T>
void PermanentTN<T>::single_out(){ //removes it from its siblings
    if(prev) prev->next = next;
    else primary_parent->pure_children_first = next;
    if(next) next->prev = prev;
    else primary_parent->pure_children_last = prev;
    //ACHTUNG a singled out child is still pointing to its old neighbours!
}

template <typename T>
StatedCollectionTN<T>* DefaultCollectionTN<T>::def_new_state() {
    if(states == 0){
        states = new StatedCollectionTN<T>(this, 0, states); //TODO give him the address of the stated out parent if exists
    }
    return states;
}

template <typename T>
void DefaultCollectionTN<T>::state_out(PermanentTN<T>* perm){
    def_new_state();
    states->add_child(perm);
}

template<typename T>
void DefaultCollectionTN<T>::restate(){ //TODO merge unpure children
    auto iter = states;
    while(1){
        if(iter->next_state == 0){
            this->pure_children_last = iter->pure_children_last;
            delete iter;
            break;
        }
        auto i2 = iter;
        iter = iter->next_state;
        delete i2;
    }
    states = 0;
}

template<typename T>
bool CollectionTN<T>::try_to_construct(Card* to_add, Identifier casted_id, Player* pl) {
    if(!fulfills(casted_id, min_chars, min_requ)) return false;
    for(auto iter = children.begin(); iter != children.end(); ++iter){
        if((*iter)->try_to_construct(to_add, casted_id, pl)) return true;
    }
    construct_pure_child(to_add, pl); //TODO: merge it with a similar child
    return true;
}

template <typename T>
void CollectionTN<T>::construct_pure_child(Card* to_add, Player* pl){
    if(pure_children_first == 0){
        pure_children_first = new PermanentTN<T>(to_add, pl, 0, 0, this); //TODO insert permanent in permanent chain
        pure_children_last = pure_children_first;
    } else {
        pure_children_first = new PermanentTN<T>(to_add, pl, 0, pure_children_first, this);
    }
}

template <typename T>
void CollectionTN<T>::add_child(PermanentTN<T>* new_child){
    if(pure_children_first == 0){
        pure_children_first = new_child;
        pure_children_last = new_child;
        new_child->prev = 0;
        new_child->next = 0;
    }
    else{
        new_child->prev = pure_children_first->prev;
        new_child->next = pure_children_first;
        if(new_child->prev) new_child->prev->next = new_child;
        pure_children_first->prev = new_child;
        pure_children_first = new_child;
    }
}

#endif //OLYMPUS_YGGDRASIL_PART_3