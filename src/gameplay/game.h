#ifndef OLYMPUS_GAMEPLAY_PROTOGAME_H
#define OLYMPUS_GAMEPLAY_PROTOGAME_H

#include "resolvables/stack.h"
#include <list>

class Gamer;

class ProtoGame {
protected:
    Stack stack;
    std::list<const Gamer*> gamers;
public:
    const Stack& getStack() const { return stack; }
    const std::list<const Gamer*>& getGamers() const { return gamers; }
};

#endif //OLYMPUS_GAMEPLAY_PROTOGAME_H
