#ifndef OLYMPUS_CLASSES_11_STATICS_H
#define OLYMPUS_CLASSES_11_STATICS_H

#include "serializable.h"

struct Modifier {
    enum type { pt, keyword } myType;
};

using Identifier = int;

class StaticAb_H {
    Identifier chars, requs;
    char nb_effects;
    Modifier first_effect;
    Modifier* other_effects; //0 +1/+1 - 1 +1/+0 - 2 +0/+1
public:
    StaticAb_H() = default;
    ~StaticAb_H() { delete[] other_effects; }

    template<bool read> friend void visit(ConstHost<StaticAb_H, read>&, Visitor<read>&);
};

struct ModifListNode{
    StaticAb_H* modif;
    ModifListNode* prev;
    ModifListNode* next;
    ModifListNode(StaticAb_H* st, ModifListNode* nxt): modif(st), prev(nullptr), next(nxt) {};
};

#endif //OLYMPUS_CLASSES_11_STATICS_H
