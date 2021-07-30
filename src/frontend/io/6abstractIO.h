#ifndef OLYMPUS_6_ABSTRACT_IO_H
#define OLYMPUS_6_ABSTRACT_IO_H

#include <vector>
#include <string>
#include <exception>
#include <headQ_rect.h>
#include "build_types.h"
#include "Mana/lib2_mana.h"

enum DirectioL{ UP, DOWN, LEFT, RIGHT, BACK, ENTER, MOUSE, NOT_RECOGNIZED};
enum framecolor { colorlessfr, whitefr, bluefr, blackfr, redfr, greenfr, multicolorfr };
//Different types of colors: uint8_types are used for communicating color guidelines/flags with the IO library,
//enum framecolors are colored frames such as "multicolor creatures"
//and chars are used as flags for a full color identity

inline enum framecolor main_color(char color){
    switch(color){
        case 0: return colorlessfr;
        case 1: return whitefr;
        case 2: return bluefr;
        case 4: return blackfr;
        case 8: return redfr;
        case 16: return greenfr;
        default: return multicolorfr;
    }
}

class UIClosedException: public std::exception{};

class ImplementIO;

class AbstractIO{
public:
    static constexpr uint8_t WHITE = 0, BLACK = 1, GREY = 2, HIGH1=10, HIGH2=11;
    static ImplementIO* god;

#define maybe_virtual virtual
#define maybe_undef = 0
#include "iomethods.cpp"
#undef maybe_virtual
#undef maybe_undef
};

#endif //OLYMPUS_6_ABSTRACT_IO_H
