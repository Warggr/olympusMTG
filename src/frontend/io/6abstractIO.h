#ifndef OLYMPUS_6_ABSTRACT_IO_H
#define OLYMPUS_6_ABSTRACT_IO_H

#include "headQ_rect.h"
#include "Mana/lib2_mana.h"
#include "build_types.h"
#include "implementIO.h"
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

class UIClosedException: public std::exception{};

class ImplementIO;

class Permanent; class Card; class Creature; class CardOracle; class EmptyOption;

class AbstractIO{
public:
    static constexpr uint8_t WHITE = 0, BLACK = 1, GREY = 2, HIGH1=10, HIGH2=11;
    thread_local static AbstractIO* god;

    AbstractIO(AbstractIO* hic) { (void) hic; god = hic; }
    virtual ~AbstractIO() = default;

#define maybe_virtual virtual
#define maybe_undef = 0
#include "iomethods.cpp"
#undef maybe_virtual
#undef maybe_undef

    template<typename O> std::list<O> checklist(std::list<O>& all, unsigned min = 0, unsigned max = 0);
    void disp(const Permanent& perm, const Rect& pos, bool highlight);
    void disp(const Creature& crea, const Rect& pos, bool highlight);
    void disp(const std::unique_ptr<Card>& card, const Rect& pos, bool highlight);
    void disp(const CardOracle& card, const Rect& pos, bool highlight);
    void disp(const EmptyOption* opt, const Rect& pos, bool highlight);
    void poster(const CardOracle& card) { disp(card, Rect(), false); }

    template<typename T>
    uint chooseAmong(std::vector<T> objects);
};

template<typename T>
std::list<T> AbstractIO::checklist(std::list<T>& all, unsigned min, unsigned max) {
    if(max == 0 and min == 0) return std::list<T>();
    Rect rect(0, 0, 0, 0);
    for (auto &obj : all) {
        disp(obj, rect, true);
        rect.shift(rect.width, 0);
    }
    std::vector<bool> check(all.size(), true);
    uint i = 0, nb_checks = 0;
    while (true) {
        switch(get_direction_key()) {
            case LEFT:
                if(i != 0) --i;
                break;
            case RIGHT:
                if(i != all.size()) ++i;
                break;
            case UP:
                check[i] = true; nb_checks++; break;
            case DOWN:
                check[i] = false; nb_checks--; break;
            case ENTER: {
                if( min > max or (min < nb_checks and nb_checks < max)) {
                    std::list<T> ret;
                    auto iter = all.begin();
                    for(uint j = 0; j < all.size(); j++) {
                        if(check[j]) {
                            auto i2 = iter;
                            iter++;
                            ret.splice(ret.end(), all, i2);
                        } else {
                            iter++;
                        }
                    }
                    return ret;
                }
            }
            default:
                break;
        }
    }
}

template<typename T>
uint AbstractIO::chooseAmong(std::vector<T> all) {
    Rect rect(0, 0, 0, 0);
    for (auto &obj : all) {
        disp(obj, rect, true);
        rect.shift(rect.width, 0);
    }
    std::vector<bool> check(all.size(), true);
    uint i = 0;
    while (true) {
        switch(get_direction_key()) {
            case LEFT:
                if(i != 0) --i;
                break;
            case RIGHT:
                if(i != all.size()) ++i;
                break;
            case ENTER:
                return i;
            default:
                break;
        }
    }
}

#endif //OLYMPUS_6_ABSTRACT_IO_H
