#ifndef OLYMPUS_YGGDRASIL_2_H
#define OLYMPUS_YGGDRASIL_2_H

#include "state_tn.h"
#include "abstract_n.h"

class BoardN;

template <typename T>
class CollectionTN: public AbstractN {
protected:
    BoardN* parent;
    std::list<StateTN<T>> children;
public:
    explicit CollectionTN(BoardN* parent): parent(parent) {}

    void insert(std::unique_ptr<Card> origin, Player* pl) {
        children.template emplace_front(std::move(origin), pl, this);
    }

    bool empty() const override { for(auto& i : children) if(!i.empty()) return false; return true; }

    StateTN<T>* defNewState() {
        return new StateTN<T>(this);
    }

    void state_out(PermanentTN<T>* perm) {
        (void) perm;//TODO
    }

//Merges every stateTN into one.
    void restate() {
        for(auto& child = children.begin().operator++(); child != children.end(); child++) {
            child->merge(*children.begin());
        }
    }

    void remove(T* object) {
        (void) object; //TODO implement
    };

    iterator<T, false> begin() { return children.front().begin(); }
    iterator<T, false> end() const { return children.back().end(); }
    iterator<T, true> cbegin() const { return children.front().cbegin(); }
    iterator<T, true> cend() const { return children.back().cend(); }
    iterator<Permanent, false> pbegin() { return children.front().pbegin(); }
    iterator<Permanent, false> pend() const { return children.back().pend(); }
    iterator<Permanent, true> cpbegin() const { return children.front().cpbegin(); }
    iterator<Permanent, true> cpend() const { return children.back().cpend(); }
};

#endif //OLYMPUS_YGGDRASIL_2_H
