#ifndef OLYMPUS_CLASSES_11_STATICS_H
#define OLYMPUS_CLASSES_11_STATICS_H

class StaticAb {
    Identifier _may, _must;
    char nb_effects;
    char first_effect;
    char* other_effects; //0 +1/+1 - 1 +1/+0 - 2 +0/+1
public:
    StaticAb() = default;
    bool init(std::ifstream& myfile);
};

struct ModifListNode{
    char nb_effects;
    char first_effect;
    char* other_effects; //0 +1/+1 - 1 +1/+0 - 2 +0/+1
    ModifListNode* next;
};

#endif //OLYMPUS_CLASSES_11_STATICS_H