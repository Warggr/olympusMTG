#ifndef OLYMPUS_HEADER_5_YGGDRASIL_H
#define OLYMPUS_HEADER_5_YGGDRASIL_H

#include <condition_variable>
#include <iterator>

template<typename T> class PermanentTN;
class PermanentN;
template<typename T> class Yggdrasil_N;
class Yggdrasil_base;
class Permanent;
template<class T, bool b> class AdapterLeaf;
template<class T, bool b> class Leaf;

template<typename T> class is_permanent: public std::false_type {};
template<> class is_permanent<Permanent>: public std::true_type {};

#define isitconst(T, iconst) typename std::conditional<iconst, const T, T>::type

template<typename T, bool iconst>
class iterator_treenode {
protected:
    iterator_treenode* parent;
public:
    constexpr iterator_treenode(iterator_treenode* parent): parent(parent) {};
    virtual ~iterator_treenode() = default;
    virtual Leaf<T, iconst>* next() = 0;
};

template<typename T, bool iconst>
class inner_iterator : public iterator_treenode<T, iconst> {
    using super = iterator_treenode<T, iconst>;
protected:
    virtual bool isEnd() const = 0;
    virtual void advance() = 0;
    virtual Leaf<T, iconst>* down() = 0;
public:
    constexpr explicit inner_iterator(inner_iterator* parent = nullptr) : super(parent) {};
    virtual ~inner_iterator() = default;
    Leaf<T, iconst>* next() override {
        advance();
        if (isEnd()) return super::parent == nullptr ? nullptr : super::parent->next();
        else return down();
    }
    //virtual bool operator==(const inner_iterator& other) const = 0;
    friend class AdapterLeaf<T, iconst>;
};

template<class T, bool b> class NullLeaf;

template<class T, bool b>
class Leaf : public iterator_treenode<T, b> {
public:
    using pointed_type = typename std::conditional<b,
            typename std::conditional<is_permanent<T>::value, const PermanentN, const PermanentTN<T>>::type,
            typename std::conditional<is_permanent<T>::value, PermanentN, PermanentTN<T>>::type
    >::type;

    constexpr Leaf(inner_iterator<T, b>* parent = nullptr): iterator_treenode<T, b>(parent) {}
    virtual bool operator==(const Leaf& other) const = 0;
    virtual pointed_type* getPointed() = 0;
};

template<class T, bool b>
class ConcreteLeaf : public Leaf<T, b> {
public:
    using pointed_type = typename Leaf<T, b>::pointed_type;
private:
    pointed_type* pted;
public:
    constexpr explicit ConcreteLeaf(pointed_type* pted, inner_iterator<T, b>* parent = nullptr):
            Leaf<T, b>(parent), pted(pted) {};
    Leaf<T, b>* next() override {
        return iterator_treenode<T, b>::parent == nullptr ? nullptr : iterator_treenode<T, b>::parent->next();
    }
    bool operator==(const Leaf<T, b>& other) const override {
        auto* a = dynamic_cast<const ConcreteLeaf<T, b>*>(&other);
        return a != nullptr and a->pted == pted;
    }
    pointed_type* getPointed() override { return pted; };
};

template<class T, bool iconst>
class AdapterLeaf: public Leaf<Permanent, iconst> {
protected:
    ConcreteLeaf<T, iconst>* content;
public:
    AdapterLeaf(ConcreteLeaf<T, iconst>* content, inner_iterator<Permanent, iconst>* parent):
        Leaf<Permanent, iconst>(parent), content(content) {};
    ~AdapterLeaf() { delete content; }
    Leaf<Permanent, iconst>* next() override {
        content = static_cast<ConcreteLeaf<T, iconst>*>(content->next());
        if(content == nullptr){
            auto* parent_cpy = this->parent;
            delete this;
            return parent_cpy->next();
        }
        else return this;
    }
    bool operator==(const Leaf<Permanent, iconst>& other) const override {
        auto* a = dynamic_cast<const AdapterLeaf<T, iconst>*>(&other);
        return ((a != nullptr) && *content == *a->content);
    }
    typename Leaf<T, iconst>::pointed_type* getPointed() override { return content->getPointed(); }
};

template<typename T, bool iconst>
class iterator {
    Leaf<T, iconst>* in;
public:
    constexpr iterator(Leaf<T, iconst>* in) : in(in) {};
    iterator operator++() { in = in->next(); return *this; }
    bool operator==(const iterator& other) const { return *in == *other.in; }
    bool operator==(const nullptr_t&) const { return in == nullptr; }
    bool operator!=(const iterator& other) const { return !operator==(other); }
    typename std::conditional<iconst, const T, T>::type* operator->() {
        return in->getPointed();
    }
    typename std::conditional<iconst, const T, T>::type& operator*() {
        return *(operator->());
    }
    typename ConcreteLeaf<T, iconst>::pointed_type* get_pointed() const { return in->getPointed(); };
};

/*template<typename T, bool iconst>
class iterator {
    using BasicCont = typename std::conditional<iconst, const PermanentTN<T>, PermanentTN<T>>::type;

    BasicCont* pointed;
    bool is_primary;
public:
    iterator(): pointed(nullptr), is_primary(false){};
    iterator(BasicCont* pted, bool primary): pointed(pted), is_primary(primary){};
    bool get_primary(){return is_primary; }
    //void set_primary(bool prim){ is_primary = prim; }
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
    BasicCont* get_pointed() const { return pointed; };
};

template<bool iconst>
class iterator<Permanent, iconst>: public std::bidirectional_iterator_tag {
    using BasicObj = typename std::conditional<iconst, const Permanent, Permanent>::type;
    using BasicCont = typename std::conditional<iconst, const AbPermanentN, AbPermanentN>::type;

    BasicCont* pointed;
    bool is_primary;
public:
    iterator(): pointed(0), is_primary(false){};
    iterator(BasicCont* pted, bool primary): pointed(pted), is_primary(primary){};
    bool get_primary(){return is_primary; }
    //void set_primary(bool prim){is_primary = prim; };
    iterator operator++() {
        pointed = pointed->pnext;
        return *this;
    }
    iterator operator--() {
        pointed = pointed->pprev;
        return *this;
    }
    iterator operator++(int){ return this->operator++(); }
    iterator operator--(int){ return this->operator--(); }
    BasicObj* operator->(){
        return &(this->operator*());  
    }
    BasicObj& operator*(){
        return pointed->get_describedObject(static_cast<BasicObj*>(nullptr));
    }
    bool operator==(const iterator& other) const {
        return (other.pointed == pointed);
    }
    bool operator!=(const iterator& other) const {
        return !(other == *this);
    }
    BasicCont* get_pointed() const {return pointed; };
};*/

#endif //OLYMPUS_HEADER_5_YGGDRASIL_H