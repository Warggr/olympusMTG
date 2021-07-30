#ifndef OLYMPUS_STACK_H
#define OLYMPUS_STACK_H

#include "5resolvables.h"
#include <memory>
#include <forward_list>

class Stack {
    std::forward_list<std::unique_ptr<Resolvable>> stack;
public:
    static Stack* god;
    Stack() { god = this; }

    void addToStack(std::unique_ptr<Resolvable> rs){ stack.push_front(std::move(rs)); };
    std::unique_ptr<Resolvable> popFromStack();
    bool stackIsEmpty(){return stack.empty(); };
    void removeFromStack(std::unique_ptr<Resolvable>& rs);

    Resolvable* iterateStack(float zImposed, DirectioL& direction);
};

#endif //OLYMPUS_STACK_H
