#ifndef OLYMPUS_FRONTEND_H
#define OLYMPUS_FRONTEND_H

#include "io/6abstractIO.h"
#include "ui/12abstractUI.h"

class FrontEnd {
    AbstractIO io;
    AbstractUI ui;
public:
    FrontEnd(): ui(&io) {};
    std::string getName() const;
};

#endif //OLYMPUS_FRONTEND_H
