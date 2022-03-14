#ifndef OLYMPUS_PLAIN_FILEREADER_H
#define OLYMPUS_PLAIN_FILEREADER_H

#include <iostream>
#include "logging.h"
#include "headE_enums.h"
#include "build_types.h" //for pedantic_safepoint
#include "classes/serializable.h"
#include "../reader.h"
#include "../visit.hpp"

class DictHolder;

class PlainFileReader: public ReaderVisitor {
private:
    DictHolder* dicts;
    enum State { go_on, invalid, breakout, end_reached } state;

    void readSectionFlavor(char*& flavor_text, uint8_t offset_text) override;
    void readSectionOthercasts(fwdlist<CardOption>& node) override;
    void readCosts(Cost& cost, bool& tapsymbol) override;
    uint nb_phrases(); //Reads how many phrases there are, separated by . and ended by '<' or '}'
    template<typename T>
    void readArray(uint& nb_objects, T*& objects) {
        nb_objects = nb_phrases();
        objects = new T[nb_objects];
        for(uint i=0; i<nb_objects; i++) {
            ::visit<true>(objects[i], *this);
        }
    }

/*    void readNumberOfObjects(uint& nb) override { nb = nb_phrases(); }
    void readNumberOfObjects(uint8_t& nb) override { nb = nb_phrases(); }*/

    void checkSafepoint(char expected, const char* error_msg);
    void raiseError(const std::string& message) override;
    inline void consumeWhitespace() { while(ifile.peek() == ' ') ifile.get(); }
    void readAtomEffect(effect_type& type, flag_t*& params, uint8_t& nbparams, char* param_hashtable);
public:
    PlainFileReader(DictHolder* dicts, std::istream& istream): ReaderVisitor(istream), dicts(dicts), state(go_on) {};

    void visit(const char*, std::string& name) override;
    void visit(const char*, Cost& cost) override;
    void visit(const char*, card_type& type) override;
    void visit(const char*, trig_type& type) override;
    void visit(const char*, char& value) override;
    void visit(const char*, Mana& mana) override;
    void visit(const char*, bool& b) override;
    void readAll(RulesHolder& rules, card_type type) override;
    void readEffect(std::forward_list<AtomEffect_H>& effects, uint8_t &nbparams, char*& param_hashtable) override;

    effect_type readAbilityType();
    short int readNumber(char a, bool can_be_zero, bool can_be_negative);
    flag_t readAbilityParameter(char* allassignedvariables, uint8_t& effect_params, flag_t type);

    void readModifier(char& nbEffects, Modifier& firstEffect, Modifier*& otherEffects) override;
    bool read_one_criterion(Identifier& chars, Identifier& requs);
    void readSelector(Identifier& chars, Identifier& requs) override;

    void readActAb(Cost& cost, Effect_H* effects, bool& tapsymbol, bool& ismanaability, bool& instantspeed) override;
    void readMainSpell(Cost& cost, Effect_H*& effect) override;
};

#ifdef F_STRICT
#define pedantic_safepoint(a, b) check_safepoint(a, b)
#else
#define pedantic_safepoint(a, b) ifile.get()
#endif

#endif //OLYMPUS_PLAIN_FILEREADER_H
