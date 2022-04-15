#ifndef OLYMPUS_JSONWRITER_H
#define OLYMPUS_JSONWRITER_H

#include "../writer.h"
#include "classes/serializable.h"
#include "Mana/cost.h"
#include <memory>

class JsonWriter : public WriterVisitor {
    bool comma {false};
    const bool pretty;
    unsigned char indent {0};

    void write(const std::string& value) { ofile << '"' << value << '"'; }
    void write(int value) { ofile << value; }
    void write(trig_type type);
    void write(const card_type& type);
    void write(const Mana& mana) { ofile << mana.m2t(); }
    void write(const Serializable& ser) { ser.write(*this); }
    void write(bool b) { if(b) ofile.write("True", 4); else ofile.write("False", 5); }
    void writeKey(const char* key){
        if(comma) ofile.put(',');
        if(pretty){
            ofile.put('\n');
            for(uint i=0; i<indent; i++) ofile.put('\t');
        }
        ofile << '"' << key << '"' << ':';
        if(pretty) ofile.put(' ');
    }
    template<typename T>
    void _visit(const char* key, T& value) {
        writeKey(key);
        write(value);
        comma = true;
    }
    void startObj();
    void endObj();
public:
    JsonWriter(std::ostream& output, bool indent): WriterVisitor(output), pretty(indent) {}
    void visit(const char* key, const std::string& name) override { _visit(key, name); }
    void visit(const char* key, trig_type type) override { _visit(key, type); }
    void visit(const char* key, const Cost& cost) override { _visit(key, cost); }
    void visit(const char* key, const card_type& type) override { _visit(key, type); }
    void visit(const char* key, const Effect_H* effect) override;
    void visit(const char* key, Mana mana) override { _visit(key, mana); }
    void visit(const char* key, bool b) override { _visit(key, b); }
    void readSectionFlavor(const char* flavor_text, uint8_t offset_text) override;
    void readSectionOthercasts(const fwdlist<CardOption>& node) override;
    void readAll(RulesHolder& rules, card_type type) override;

    void readEffectH(uint8_t& nb_params, char*& params, std::forward_list<AtomEffect_H>& atoms);
    void readAtomEffect(effect_type& type, flag_t*& params, uint8_t& nbparams, char* param_hashtable) override;
    void readActAb(Cost& cost, Effect_H* effects, bool& tapsymbol, bool& ismanaability, bool& instantspeed) override;

    void readMainSpell(Cost& cost, Effect_H*& effect) override;
    void readSelector(Identifier& chars, Identifier& requs) override;
    void readModifier(char& i, Modifier& first_effect, Modifier*& other_effects) override;
    void readCosts(Cost& costs, bool& tapsymbol) override;
};

#endif //OLYMPUS_JSONWRITER_H
