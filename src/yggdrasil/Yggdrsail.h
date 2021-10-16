#ifndef OLYMPUS_YGGDRSAIL_H
#define OLYMPUS_YGGDRSAIL_H

#include "Object.h"
#include "Node.h"
#include "Yggdrasil.h"

class Y_Hashtable : public Yggdrasil {
    int ht_size;
    Node* ht;
public:
    class iterator : public inner_iterator {
        unsigned char position, order, multiplicity;
        Y_Hashtable* node;
    public:
        explicit iterator(Y_Hashtable* n, inner_iterator* parent = nullptr) :
            inner_iterator(parent), position(0), order(1), multiplicity(0), node(n) {};
        void advance() override { position++; }
        bool isEnd() const override { return position * order > node->ht_size; }
        Yggdrasil& getPointed() override { return node->ht[position * order]; }
    };

    Y_Hashtable(): ht_size(1), ht(new Node[1]) { }
    void splitUp(Criterion crit) {
        Node* new_ht = new Node[2*ht_size];
        for(int i=0; i<ht_size; i++) {
            ht[i].slice_in_two(crit, new_ht + i, new_ht + ht_size + i);
        }
        ht_size = 2*ht_size;
        delete[] ht;
        ht = new_ht;
    }
    Yggdrasil::iterator begin() override {
        return Yggdrasil::iterator(new iterator(this));
    }
    inner_iterator* createStart(inner_iterator* parent) override {
        return ht[0].createStart(new iterator(this, parent));
    }
};

class Y_Dichotomy : public Yggdrasil {
    Yggdrasil* parts[2] { nullptr };
public:
    class iterator : public inner_iterator {
        Y_Dichotomy* node;
        char pos : 1;
    public:
        iterator(Y_Dichotomy* node, inner_iterator* parent): inner_iterator(parent), node(node), pos(0) {}
        bool isEnd() const override { return pos == 1; }
        void advance() override { pos = 1; }
        Yggdrasil& getPointed() override { return *node->parts[pos]; }
    };

    Yggdrasil::iterator begin() override {
        return Yggdrasil::iterator(new iterator(this, nullptr));
    }
    inner_iterator* createStart(inner_iterator *parent) override {
        return parts[0]->createStart(new iterator(this, parent));
    }
};

#endif //OLYMPUS_YGGDRSAIL_H
