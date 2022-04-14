#ifndef OLYMPUS_DISPLAYABLE_H
#define OLYMPUS_DISPLAYABLE_H

#include <string>

class BasicIO;

class Displayable {
public:
    virtual ~Displayable() = default;
    virtual std::string describe() const = 0;
    virtual void disp(BasicIO* io) const = 0;
};

#endif //OLYMPUS_DISPLAYABLE_H
