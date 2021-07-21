ifndef OLYMPUS_HEADER_5_YGGDRASIL_H
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
    inline bool operator==(const iterator& other) const {
        return (other.pointed == pointed);
    }
    inline bool operator!=(const iterator& other) const {
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

#include "archetypes.cpp"

#endif //OLYMPUS_HEADER_5_YGGDRASIL_H