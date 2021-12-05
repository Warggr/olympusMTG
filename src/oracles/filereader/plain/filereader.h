#ifndef OLYMPUS_PLAIN_FILEREADER_H
#define OLYMPUS_PLAIN_FILEREADER_H

#include <iostream>
#include <utility>
#include "headE_enums.h"
#include "build_types.h"
#include "../visitor.h"

class DictHolder;

class PlainFileReader: public ReaderVisitor {
private:
    DictHolder* dicts;
    std::istream& ifile;
    enum State { go_on, invalid, breakout, end_reached } state;

    void read_section_flavor(char*& flavor_text, uint8_t offset_text) override;
    void read_section_othercasts(fwdlist<CardOption>& node) override;
    void readCosts(Cost& cost, bool& tapsymbol) override;

    uint nb_phrases(); //Reads how many phrases there are, separated by . and ended by '<' or '}'
    void readNumberOfObjects(uint& nb) override { nb = nb_phrases(); }
    void readNumberOfObjects(uint8_t& nb) override { nb = nb_phrases(); }

    void check_safepoint(char expected, const char* error_msg);
    void raise_error(const std::string& message) override;
    inline void consumeWhitespace() { while(ifile.peek() == ' ') ifile.get(); }
public:
    PlainFileReader(DictHolder* dicts, std::istream& istream): dicts(dicts), ifile(istream), state(go_on) {};

    void readName(std::string& name) override;
    void readCost(Cost& cost) override;
    void readCardType(card_type& type) override;
    void readAll(RulesHolder& rules, card_type type) override;

    effect_type readAbilityType();
    short int readNumber(char a, bool can_be_zero, bool can_be_negative);
    flag_t readAbilityParameter(char* allassignedvariables, uint8_t& effect_params, flag_t type);

    void readModifier(char& nbEffects, Modifier& firstEffect, Modifier*& otherEffects) override;
    bool read_one_criterion(Identifier& chars, Identifier& requs);
    void read_selector(Identifier& chars, Identifier& requs);

    void readActAb(Cost& cost, Effect_H* effects, bool& tapsymbol, bool& ismanaability, bool& instantspeed) override;
    void readEffectH(uint8_t& nb_params, char*& params, std::forward_list<AtomEffect_H>& atoms) override;
    void readAtomEffect(effect_type& type, flag_t*& params, uint8_t& nbparams, char* param_hashtable) override;

    void readTriggerType(trig_type& type) override;
    void readSelector(Identifier& chars, Identifier& requs) override;
    void readMainSpell(Cost& cost, Effect_H*& effect) override;
};

#ifdef F_STRICT
#define pedantic_safepoint(a, b) check_safepoint(a, b)
#else
#define pedantic_safepoint(a, b) ifile.get()
#endif

#endif //OLYMPUS_PLAIN_FILEREADER_H
