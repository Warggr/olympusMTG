#ifndef OLYMPUS_MOCKIO_H
#define OLYMPUS_MOCKIO_H
#include "../io/6abstractIO.h"

class MockIO : public AbstractIO {
public:
#define BOOLEAN(x) bool x { return true; }
#define INTEGER(x) int x { return 0; }
#define FLOATING(x) float x { return 0; }
#define DIRECTIONENUM(x) DirectioL x { return DirectioL::UP; }
#define STRING(x) std::string x { return std::string(); }
#define DEVOID(x) void x {}
#include "../io/iomethods.cpp"
};

#endif //OLYMPUS_MOCKIO_H
