#include "stack.h"
#include "5resolvables.h"

std::unique_ptr<Resolvable> Stack::popFromStack(){
    if(stack.empty()) return nullptr;
    auto ret = std::move(stack.front());
    stack.pop_front();
    return ret;
}

void Stack::removeFromStack(std::unique_ptr<Resolvable>& rs){
    stack.remove(rs);
}
