#ifndef OLYMPUS_FILEREADER_H
#define OLYMPUS_FILEREADER_H

#include <iostream>
#include <utility>
#include "classes/3statics.h"
#include "headE_enums.h"
#include "build_types.h"
#include "visitor.h"
#include "dictholder.h"

class WeirdCost;

class PlainFileReader: public ReaderVisitor {
private:
    DictHolder* dicts;
    std::istream& ifile;

    void read_section_flavor(char*& flavor_text, uint8_t offset_text) override;
    void readCosts(Mana& mana, bool& tapsymbol, WeirdCost*& others) override;

    uint nb_phrases(char ending_character); //Reads how many phrases there are, separated by ';' and ended by '<'
    uint readNumberOfObjects() override { return nb_phrases(';'); }

    void check_safepoint(char expected, const char* error_msg);
    static void raise_error(const std::string& message);
    inline void consumeWhitespace() { while(ifile.peek() == ' ') ifile.get(); }
public:
    PlainFileReader(DictHolder* dicts, std::istream& istream): dicts(dicts), state(go_on), ifile(istream) {};

    void readName(std::string& name) override;
    void readManaCost(Mana& mana) override;
    void readCardType(card_type& type) override;
    void readAll(RulesHolder& rules, card_type type) override;

    effect_type readAbilityType();
    short int readNumber(char a, bool can_be_zero, bool can_be_negative);
    flag_t readAbilityParameter(char* allassignedvariables, uint8_t& effect_params, flag_t type);

    void readModifier(char &nbEffects, Modifier &firstEffect, Modifier *&otherEffects) override;
    bool read_one_criterion(Identifier &chars, Identifier &requs);
    void read_selector(Identifier &chars, Identifier &requs);

    enum State { go_on, invalid, breakout, end_reached } state;

    void readActAb(Mana &mana, Effect_H *&effects, bool &tapsymbol, bool &ismanaability) override;
    void readEffectH(uint8_t &nb_params, char *&params, std::forward_list<AtomEffect_H> &atoms) override;
    void readAtomEffect(effect_type& type, flag_t*& params, uint8_t& nbparams, char* param_hashtable) override;

    void readTriggerType(trigtype& type) override;

    void read_section_onresolve(Effect_H*& preRes) override {
        if(preRes != nullptr) raise_error("OnResolve abilities of this spell declared multiple times");
        preRes = new Effect_H(*this);
    }
};

#endif //OLYMPUS_FILEREADER_H
