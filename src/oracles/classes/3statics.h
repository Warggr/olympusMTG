#ifndef OLYMPUS_CLASSES_11_STATICS_H
#define OLYMPUS_CLASSES_11_STATICS_H

struct Modifier {
    enum type { plus_power, plus_toughness, plus_oneplusone, keyword } myType;
};

using Identifier = int;
class ReaderVisitor;

class StaticAb_H {
    Identifier chars, requs;
    char nb_effects;
    Modifier first_effect;
    Modifier* other_effects; //0 +1/+1 - 1 +1/+0 - 2 +0/+1
public:
    StaticAb_H() = default;
    void init(ReaderVisitor& myfile);
};

struct ModifListNode{
    char nb_effects;
    Modifier first_effect;
    Modifier* other_effects;
    ModifListNode* next;
};

#endif //OLYMPUS_CLASSES_11_STATICS_H