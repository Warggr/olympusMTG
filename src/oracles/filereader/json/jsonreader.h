#ifndef OLYMPUS_BINARYREADER_H
#define OLYMPUS_BINARYREADER_H

#include <iostream>
#include "../reader.h"

class JsonReader : public ReaderVisitor {
    inline void canary(char){}
public:
    explicit JsonReader(std::istream& stream): ReaderVisitor(stream) {};
    void raise_error(const std::string& message) override;
    void readName(std::string& name) override;
    void readCost(Cost& cost) override;
    void readCardType(card_type& type) override;
    void readSectionFlavor(char*& flavor_text, uint8_t offset_text) override;
    void readSectionOthercasts(fwdlist<CardOption>& node) override;

    template<typename T>
    void directRead(T obj, const char* name) {
        char begin; std::skipws(ifile); ifile >> begin;
        if(begin != '{') throw DeckbuildingError("JSON error: does not start with '{'");
        std::string key;
    }

    template<>
    void directRead(int obj, const char* name) {
        char begin; std::skipws(ifile); ifile >> begin;
        if(begin != '{') throw DeckbuildingError("JSON error: does not start with '{'");
        std::string key;
    }

    void readAll(RulesHolder& rules, card_type type) override;

    void readNumberOfObjects(uint& nb) override;
    void readNumberOfObjects(uint8_t& nb) override;

    void readEffectH(uint8_t &nb_params, char*& params, std::forward_list<AtomEffect_H>& atoms) override;
    void readTriggerType(trig_type& type) override;
    void readAtomEffect(effect_type& type, flag_t*& params, uint8_t& nbparams, char* param_hashtable) override;
    void readActAb(Cost& cost, Effect_H* effects, bool &tapsymbol, bool &ismanaability, bool& instantspeed) override;

    void readMainSpell(Cost& cost, Effect_H*& effect) override;

    void readSelector(Identifier& chars, Identifier& requs) override;
    void readModifier(char& i, Modifier& first_effect, Modifier*& other_effects) override;
    void readCosts(Cost& cost, bool& tapsymbol) override;
};

#endif //OLYMPUS_BINARYREADER_H
