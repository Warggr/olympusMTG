#ifndef OLYMPUS_WRITER_H
#define OLYMPUS_WRITER_H

#include "headI_identifiers.h"
#include "headE_enums.h"
#include "visitor.h"
#include <ostream>
#include <forward_list>

class CardOption; class Cost; class RulesHolder; class Modifier; class Effect_H;
class AtomEffect_H; class Mana;

class WriterVisitor {
protected:
    std::ostream& ofile;
    /*template<typename T>
    void readArray(uint& nb_objects, T*& objects) {
        readNumberOfObjects(nb_objects);
        for(uint i=0; i<nb_objects; i++) {
            objects[i].write(*this);
        }
    }*/
    virtual void raiseError(const std::string& message) = 0;
    virtual void readSectionFlavor(const char* flavor_text, uint8_t offset_text) = 0;
    virtual void readSectionOthercasts(const fwdlist<CardOption>& node) = 0;
public:
    WriterVisitor(std::ostream& output): ofile(output) {};
    virtual ~WriterVisitor() = default;
    virtual void visit(const char* key, bool b) = 0;
    virtual void visit(const char* key, char value) = 0;
    virtual void visit(const char* key, const std::string& str) = 0;
    virtual void visit(const char* key, const card_type& type) = 0;
    virtual void visit(const char* key, trig_type type) = 0;
    virtual void visit(const char* key, const Cost& cost) = 0;
    virtual void visit(const char* key, Mana mana) = 0;

    virtual void readAll(const RulesHolder& rules, const card_type& type) = 0;
    virtual void readEffect(const std::forward_list<AtomEffect_H>& effects, uint8_t nbparams, const char* param_hashtable) = 0;

    virtual void readAtomEffect(effect::type type, const flag_t* params) = 0;
    virtual void readActAb(const Cost& cost, const Effect_H* effects, bool tapsymbol, bool ismanaability, bool instantspeed) = 0;
    virtual void readSelector(Identifier chars, Identifier requs) = 0;
    virtual void readModifier(char i, const Modifier& first_effect, const Modifier* other_effects) = 0;
    virtual void readCosts(const Cost& cost, bool tapsymbol) = 0;
    virtual void readSubtypes() = 0;

    virtual void readMainSpell(const Cost& cost, const Effect_H* effects) = 0;
};

#endif //OLYMPUS_WRITER_H
