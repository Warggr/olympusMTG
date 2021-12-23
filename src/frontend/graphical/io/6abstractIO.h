#ifndef OLYMPUS_6_ABSTRACT_IO_H
#define OLYMPUS_6_ABSTRACT_IO_H

#include "frontend/headQ_rect.h"
#include "frontend/basicIO.h"
#include "Mana/lib2_mana.h"
#include "frontend/basicIO.h"
#include "build_types.h"
#include <vector>
#include <string>
#include <list>
#include <exception>
#include <memory>

enum framecolor { colorlessfr, whitefr, bluefr, blackfr, redfr, greenfr, multicolorfr };
//Different types of colors: uint8_types are used for communicating color guidelines/flags with the IO myLibrary,
//enum framecolors are colored frames such as "multicolor creatures"
//and chars are used as flags for a full color identity

inline enum framecolor main_color(colorId::type color){
    switch(color.fields){
        case 0: return colorlessfr;
        case colorId::white.fields: return whitefr;
        case colorId::blue.fields: return bluefr;
        case colorId::black.fields: return blackfr;
        case colorId::red.fields: return redfr;
        case colorId::green.fields: return greenfr;
        default: return multicolorfr;
    }
}

class Permanent; class CardWrapper; class Creature; class CardOracle; class Player; class Resolvable;

class AbstractIO : public BasicIO {
public:
    static constexpr uint8_t WHITE = 0, BLACK = 1, GREY = 2, HIGH1=10, HIGH2=11;

    virtual ~AbstractIO() = default;

    static AbstractIO* factory();

#define maybe_virtual virtual
#define maybe_undef = 0
#include "iomethods.cpp"

#undef maybe_virtual
#undef maybe_undef

    void disp(const CardOracle&, int) const;
    checklistCallbackAction getNextPosition(abstract_iterator_wrapper*, uint&, uint);
    void draw(const CardWrapper& card, const Rect& rect, bool highlight);
    virtual void focus() {};
};

#endif //OLYMPUS_6_ABSTRACT_IO_H
