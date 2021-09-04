#ifndef OLYMPUS_YGGDRSAIL_H
#define OLYMPUS_YGGDRSAIL_H

#include "Object.h"
#include "Node.h"
#include "Yggdrasil.h"

class Y_Hashtable : public Yggdrasil {
    int ht_size;
    Node* ht;
public:
    class iterator : public Yggdrasil::iterator {
        unsigned char position, order, multiplicity;
    };
    void splitUp(Criterion crit) {
        Node* new_ht = new Node[2*ht_size];
        for(int i=0; i<ht_size; i++) {
            ht[i].slice_in_two(crit, new_ht + i, new_ht + ht_size + i);
        }
        ht_size = 2*ht_size;
        delete[] ht;
        ht = new_ht;
    }
};

class Y_Dichotomy : public Yggdrasil {
    Yggdrasil* yes, * no;
public:
    class iterator : public Yggdrasil::iterator {
        bool yesorno;
    };
};

#endif //OLYMPUS_YGGDRSAIL_H
