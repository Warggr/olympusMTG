#ifndef OLYMPUS_VISITOR_H
#define OLYMPUS_VISITOR_H

#include "headE_enums.h"

static constexpr uint defaultOffsetFor(const card_type& type) {
    return (type.underlying == card_type::creature) ? 2 :
           (type.underlying == card_type::planeswalker) ? 1 : 0; //2 for creatures, 1 for planeswalkers
}

class DeckbuildingError: public std::exception {
    std::string descr;
public:
    explicit DeckbuildingError(std::string what): descr(std::move(what)) {};
    const char* what() const noexcept override { return descr.c_str(); }
};

#endif //OLYMPUS_VISITOR_H
