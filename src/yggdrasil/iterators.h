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

#define isitconst(T, iconst) typename std::conditional<iconst, const T, T>::type

/** Abstract class from which all iterators derive. */
template<typename T, bool iconst>
class iterator_treenode {
protected:
    iterator_treenode* parent;
public:
    constexpr iterator_treenode(iterator_treenode* parent): parent(parent) {};
    virtual ~iterator_treenode() = default;
    virtual Leaf<T, iconst>* next(bool bk) = 0;
};

template<typename T, bool iconst>
class inner_iterator : public iterator_treenode<T, iconst> {
    using super = iterator_treenode<T, iconst>;
protected:
    virtual bool isEnd(bool bk) const = 0;
    virtual void advance(bool bk) = 0;
    virtual Leaf<T, iconst>* down(bool bk) = 0;
public:
    constexpr explicit inner_iterator(inner_iterator* parent = nullptr) : super(parent) {};
    virtual ~inner_iterator() = default;
    Leaf<T, iconst>* next(bool bk) override {
        advance(bk);
        if (isEnd(bk)) return super::parent == nullptr ? nullptr : super::parent->next(bk);
        else return down(bk);
    }
    //virtual bool operator==(const inner_iterator& other) const = 0;
    friend class AdapterLeaf<T, iconst>;
};

template<class T, bool b>
class Leaf : public iterator_treenode<T, b> {
public:
    using pointed_type = typename std::conditional<b,
            typename std::conditional<std::is_same<T, Permanent>::value, const PermanentN, const PermanentTN<T>>::type,
            typename std::conditional<std::is_same<T, Permanent>::value, PermanentN, PermanentTN<T>>::type
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
    Leaf<T, b>* next(bool bk) override {
        return iterator_treenode<T, b>::parent == nullptr ? nullptr : iterator_treenode<T, b>::parent->next(bk);
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
    Leaf<Permanent, iconst>* next(bool bk) override {
        content = static_cast<ConcreteLeaf<T, iconst>*>(content->next(bk));
        if(content == nullptr){
            auto* parent_cpy = this->parent;
            delete this;
            return parent_cpy->next(bk);
        }
        else return this;
    }
    bool operator==(const Leaf<Permanent, iconst>& other) const override {
        auto* a = dynamic_cast<const AdapterLeaf<T, iconst>*>(&other);
        return ((a != nullptr) && *content == *a->content);
    }

    isitconst(PermanentN*, iconst) getPointed() override { return content->getPointed(); }
};

/** Allows to iterate over an Yggdrasil object. */
template<typename T, bool iconst>
class iterator {
    /** State pattern: under the hood, you're not dealing with one object that moves,
     * but with a hierarchy of Leaves that get built and destroyed. */
    Leaf<T, iconst>* in;
public:
    constexpr iterator(Leaf<T, iconst>* in) : in(in) {};
    iterator operator--() { in = in->next(false); return *this; }
    iterator operator++() { in = in->next(true); return *this; }
    bool operator==(const iterator& other) const { return in == other.in or *in == *other.in; }
    bool operator!=(const iterator& other) const { return !operator==(other); }
    template<typename S = T>
    typename std::enable_if<!std::is_same<S, Permanent>::value,
    typename std::conditional<iconst, const T, T>::type>::type* operator->() {
        return in->getPointed()->getObject();
    }
    template<typename S = T>
    typename std::enable_if<std::is_same<S, Permanent>::value,
            typename std::conditional<iconst, const T, T>::type>::type* operator->() {
        return in->getPointed()->getPermanent();
    }
    typename std::conditional<iconst, const T, T>::type& operator*() {
        return *(operator->());
    }
    typename ConcreteLeaf<T, iconst>::pointed_type* getPointed() const { return in->getPointed(); };
};

#endif //OLYMPUS_HEADER_5_YGGDRASIL_H