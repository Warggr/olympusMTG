#ifndef OLYMPUS_VISITOR_H
#define OLYMPUS_VISITOR_H

#include <forward_list>
#include "headI_identifiers.h"

class CardOption; class Cost; class RulesHolder; class Modifier; class Effect_H;
class AtomEffect_H;

class ReaderVisitor {
public:
    virtual ~ReaderVisitor() = default;
protected:
    template<typename T>
    void readArray(uint& nb_objects, T*& objects, bool create_new) {
        readNumberOfObjects(nb_objects);
        if(nb_objects != 0 && create_new) objects = new T[nb_objects];
        for(uint i=0; i<nb_objects; i++) {
            objects[i].init(*this);
        }
    }

    virtual void raise_error(const std::string& message) = 0;
    virtual void readNumberOfObjects(uint& nb) = 0;
    virtual void readNumberOfObjects(uint8_t& nb) = 0;
    static constexpr uint defaultOffsetFor(card_type type) {
        return (type.underlying == card_type::creature) ? 2 :
        (type.underlying == card_type::planeswalker) ? 1 : 0; //2 for creatures, 1 for planeswalkers
    }
    virtual void read_section_flavor(char*& flavor_text, uint8_t offset_text) = 0;
    virtual void read_section_othercasts(fwdlist<CardOption>& node) = 0;
public:
    virtual void readName(std::string& name) = 0;
    virtual void readCost(Cost& cost) = 0;
    virtual void readCardType(card_type& type) = 0;

    virtual void readAll(RulesHolder& rules, card_type type) = 0;

    virtual void readEffectH(uint8_t& nb_params, char *&params, std::forward_list<AtomEffect_H>& atoms) = 0;
    virtual void readTriggerType(trig_type& type) = 0;
    virtual void readAtomEffect(effect::type& type, flag_t*& params, uint8_t& nbparams, char* param_hashtable) = 0;
    virtual void readActAb(Cost& cost, Effect_H* effects, bool& tapsymbol, bool& ismanaability, bool& instantspeed) = 0;
    virtual void readSelector(Identifier& chars, Identifier& requs) = 0;
    virtual void readModifier(char& i, Modifier& first_effect, Modifier*& other_effects) = 0;
    virtual void readCosts(Cost& cost, bool& tapsymbol) = 0;

    virtual void readMainSpell(Cost& cost, Effect_H*& effects) = 0;
};

class DeckbuildingError: public std::exception {
    std::string descr;
public:
    explicit DeckbuildingError(std::string what): descr(std::move(what)) {};
    const char* what() const noexcept override { return descr.c_str(); }
};

#endif //OLYMPUS_VISITOR_H
