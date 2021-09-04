#ifndef OLYMPUS_YGGDRASIL_H
#define OLYMPUS_YGGDRASIL_H

class Yggdrasil {
protected:
    class inner_iterator {
    protected:
        inner_iterator* parent;
    public:
        explicit iterator(inner_iterator* parent) : parent(parent) {};
        virtual iterator operator--() = 0;
        virtual iterator operator++() = 0;
    };
    class iterator {
        inner_iterator* in;
        virtual iterator operator--() = 0;
        virtual iterator operator++() = 0;
    };
public:
    virtual iterator begin() = 0;
    virtual iterator end() = 0;
};

#endif //OLYMPUS_YGGDRASIL_H
