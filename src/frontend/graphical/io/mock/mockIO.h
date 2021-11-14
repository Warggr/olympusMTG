#ifndef OLYMPUS_MOCKIO_H
#define OLYMPUS_MOCKIO_H
#include "frontend/graphical/io/6abstractIO.h"

class MockIO : public AbstractIO {
    template<typename T> T defaultValue() const { return T(); }
public:
#define METHOD(type, x) type x { return defaultValue<type>(); }
#include "../iomethods.cpp"

};

#endif //OLYMPUS_MOCKIO_H
