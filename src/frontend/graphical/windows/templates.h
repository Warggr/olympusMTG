#ifndef OLYMPUS_UI_TEMPLATES_H
#define OLYMPUS_UI_TEMPLATES_H

#include "window.h"

template<typename Container, typename T>
struct ContainerPolicy {
    static constexpr bool orientation = UIElement::vertical;
    Container subWins;

    using iterator = typename Container::iterator;
    using const_iterator = typename Container::const_iterator;

    void insert(const T& t) { subWins.push_back(t); }

    void setCoords(const Rect&) {};

    iterator begin() { return subWins.begin(); }
    iterator end() { return subWins.end(); }
    const_iterator begin() const { return subWins.begin(); }
    const_iterator end() const { return subWins.end(); }
};

template<typename T> struct VectorElement: public TemplateGallery<ContainerPolicy<std::vector<T>, T>> {
    void insert(const T& t) { this->policy.insert(t); }
};
template<typename T> struct ListElement: public TemplateGallery<ContainerPolicy<std::list<T>, T>> {
    void insert(const T& t) { this->policy.insert(t); }
};

template<typename Class1, typename Class2, bool default_orientation>
struct PolicyWith2Objects {
    static constexpr bool orientation = default_orientation;
    Class1 object1;
    Class2 object2;

    void setCoords(const Rect&) {};

    struct iterator {
        PolicyWith2Objects* owner;
        uint where;
        iterator() = default;
        iterator(PolicyWith2Objects* owner, uint where): owner(owner), where(where) {};
        iterator& operator++() { where++; return *this; }
        iterator& operator--() { where--; return *this; }
        bool operator==(const iterator& other) const { return other.where == where and other.owner == owner; }
        bool operator!=(const iterator& other) const { return !operator==(other); }
        UIElement& operator*() const { return *operator->(); }
        UIElement* operator->() const {
            switch(where) {
                case 0: return &owner->object1;
                case 1: return &owner->object2;
                default: return nullptr;
            }
        }
    };

    iterator begin() { return iterator(this, 0); }
    iterator begin() const { return iterator(const_cast<PolicyWith2Objects*>(this), 0); }
    iterator end() { return iterator(this, 2); }
    iterator end() const { return iterator(const_cast<PolicyWith2Objects*>(this), 2); }
};

template<typename Class1, typename Class2, typename Class3, bool default_orientation>
struct PolicyWith3Objects {
    static constexpr bool orientation = default_orientation;
    Class1 object1;
    Class2 object2;
    Class3 object3;

    void setCoords(const Rect&) {};

    struct iterator {
        PolicyWith3Objects* owner;
        uint where;
        iterator() = default;
        iterator(PolicyWith3Objects* owner, uint where): owner(owner), where(where) {};
        bool operator==(const iterator& other) const { return other.where == where and other.owner == owner; }
        bool operator!=(const iterator& other) const { return !operator==(other); }
        iterator& operator++() { where++; return *this; }
        iterator& operator--() { where--; return *this; }
        UIElement& operator*() const { return *operator->(); }
        UIElement* operator->() const {
            switch(where) {
                case 0: return &owner->object1;
                case 1: return &owner->object2;
                case 2: return &owner->object3;
                default: return nullptr;
            }
        }
    };
    iterator begin() { return iterator(this, 0); }
    iterator begin() const { return iterator(const_cast<PolicyWith3Objects*>(this), 0); }
    iterator end() { return iterator(this, 3); }
    iterator end() const { return iterator(const_cast<PolicyWith3Objects*>(this), 3); }
};

#endif //OLYMPUS_UI_TEMPLATES_H