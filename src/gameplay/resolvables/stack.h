#ifndef OLYMPUS_STACK_H
#define OLYMPUS_STACK_H

#include "5resolvables.h"
#include <memory>
#include <forward_list>

class Stack {
    using stack_t = std::forward_list<std::unique_ptr<Resolvable>>;
    stack_t stack;
public:
    static Stack* god;
    Stack() { god = this; }

    void addToStack(std::unique_ptr<Resolvable> rs){ stack.push_front(std::move(rs)); };
    std::unique_ptr<Resolvable> popFromStack();
    bool stackIsEmpty(){return stack.empty(); };
    void removeFromStack(Resolvable* rs);
    stack_t::const_iterator begin() const { return stack.begin(); }
    stack_t::const_iterator end() const { return stack.end(); }
    uint size() const { return std::distance(stack.begin(), stack.end()); }

//    Resolvable* iterateStack(float zImposed, DirectioL& direction);
//    void disp_stack() const;
//    void disp_preRes(const Effect_H* preRes, const std::string& origin_name) const;
};

#endif //OLYMPUS_STACK_H
