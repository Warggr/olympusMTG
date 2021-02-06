#ifndef OLYMPUS_HEADER_6_ITERATORS_H
#define OLYMPUS_HEADER_6_ITERATORS_H

#include <iostream>
#include "head5_board.h"

class Permanent;

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
#endif //OLYMPUS_HEADER_6_ITERATORS_H