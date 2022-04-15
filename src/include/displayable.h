#ifndef OLYMPUS_DISPLAYABLE_H
#define OLYMPUS_DISPLAYABLE_H

#include <string>

class Canvas;

typedef bool disp_flags;

class Displayable {
public:
    virtual ~Displayable() = default;
    virtual std::string describe() const = 0;
    virtual void disp(Canvas* io, disp_flags flags = 0) const = 0;
};

#endif //OLYMPUS_DISPLAYABLE_H
