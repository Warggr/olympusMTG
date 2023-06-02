#ifndef OLYMPUS_READER_H
#define OLYMPUS_READER_H

#include "headI_identifiers.h"
#include "headE_enums.h"
#include "visitor.h"
#include <istream>
#include <forward_list>
#include <cstdint>

class CardOption; class Cost; class RulesHolder; class Modifier; class Effect_H;
class AtomEffect_H; class Mana;

class ReaderVisitor {
protected:
    std::istream& ifile;
    /*template<typename T>
    void readArray(uint& nb_objects, T*& objects) {
        readNumberOfObjects(nb_objects);
        if(nb_objects != 0) objects = new T[nb_objects];
        for(uint i=0; i<nb_objects; i++) {
            objects[i].write(*this);
        }
    }*/
    virtual void raiseError(const std::string& message) = 0;
    virtual void readSectionFlavor(char*& flavor_text, uint8_t offset_text) = 0;
    virtual void readSectionOthercasts(fwdlist<CardOption>& node) = 0;
public:
    ReaderVisitor(std::istream& input): ifile(input) {};
    virtual ~ReaderVisitor() = default;
    virtual void visit(const char* key, bool& b) = 0;
    virtual void visit(const char* key, char& value) = 0;
    virtual void visit(const char* key, std::string& str) = 0;
    virtual void visit(const char* key, card_type& type) = 0;
    virtual void visit(const char* key, trig_type& type) = 0;
    virtual void visit(const char* key, Cost& cost) = 0;
    virtual void visit(const char* key, Mana& mana) = 0;
//    virtual void visit(const char* key, Effect_H*& effect) = 0;
    virtual void readEffect(std::forward_list<AtomEffect_H>& effects, uint8_t& nbparams, char*& param_hashtable) = 0;

    virtual void readAll(RulesHolder& rules, card_type type) = 0;

    virtual void readActAb(Cost& cost, Effect_H* effects, bool& tapsymbol, bool& ismanaability, bool& instantspeed) = 0;
    virtual void readSelector(Identifier& chars, Identifier& requs) = 0;
    virtual void readModifier(char& i, Modifier& first_effect, Modifier*& other_effects) = 0;
    virtual void readCosts(Cost& cost, bool& tapsymbol) = 0;
    virtual void readSubtypes();

    virtual void readMainSpell(Cost& cost, Effect_H*& effects) = 0;
};

#endif //OLYMPUS_READER_H
