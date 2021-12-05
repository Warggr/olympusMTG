#include "stack.h"
#include "5resolvables.h"
#include <cassert>

Stack* Stack::god = nullptr;

std::unique_ptr<Resolvable> Stack::popFromStack(){
    if(stack.empty()) return nullptr;
    auto ret = std::move(stack.front());
    stack.pop_front();
    return ret;
}

void Stack::removeFromStack(Resolvable* rs){
    for(auto iter = stack.begin(); iter != stack.end(); iter++) {
        if(iter._M_next() != stack.end() and iter._M_next()->get() == rs) {
            stack.erase_after(iter);
        }
    }
    assert(false);
}
