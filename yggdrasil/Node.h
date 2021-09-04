#ifndef OLYMPUS_NODE_H
#define OLYMPUS_NODE_H

#include <list>
#include "Object.h"
#include "Yggdrasil.h"

class Node : public Yggdrasil {
    std::list<Object> objs;
public:
    class iterator : public Yggdrasil::iterator {
        std::list<Object>::iterator it;
    };
    void slice_in_two(Criterion crit, Node* fulfilled, Node* not_fulfilled) {
        for(auto i = objs.begin(); i != objs.end(); ++i) {
            if(i->fulfills(crit)) {
                not_fulfilled->objs.splice(not_fulfilled->objs.begin(), objs, i);
            }
        }
        fulfilled->objs = std::move(objs);
    }
    Yggdrasil::iterator*
};

#endif //OLYMPUS_NODE_H
