#ifndef OLYMPUS_BINARYREADER_H
#define OLYMPUS_BINARYREADER_H

#include "../reader.h"
#include "../visit.hpp"
#include "classes/serializable.h"

class BinaryReader : public ReaderVisitor {
    template<typename T>
    void directRead(T& value) {
        ifile.read(reinterpret_cast<char*>(&value), sizeof value);
    }
    template<typename T>
    void readArray(uint& nb_objects, T*& objects) {
        directRead(nb_objects);
        if(nb_objects != 0) objects = new T[nb_objects];
        for(uint i=0; i<nb_objects; i++) {
            ::visit<true>(objects[i],* this);
        }
    }
protected:
    void raiseError(const std::string& message) override;
    void readAtomEffect(effect_type& type, flag_t*& params);
public:
    explicit BinaryReader(std::istream& stream): ReaderVisitor(stream) {};
    void visit(const char* key, std::string& name) override;
    void visit(const char*, Cost& cost) override;
    void visit(const char*, card_type& type) override { directRead<>(type); }
    void visit(const char*, trig_type& type) override { directRead<>(type); }
    void visit(const char*, bool& b) override { directRead(b); }
    void visit(const char*, Mana& mana) override { directRead(mana); }
    void visit(const char*, char& value) override { directRead(value); }
    void readSectionFlavor(char*& flavor_text, uint8_t offset_text) override;
    void readSectionOthercasts(fwdlist<CardOption>& node) override;

    void readAll(RulesHolder& rules, card_type type) override;
    void readEffect(std::forward_list<AtomEffect_H>& effects, uint8_t& nbparams, char*& param_hashtable) override;

    void readActAb(Cost& cost, Effect_H* effects, bool &tapsymbol, bool &ismanaability, bool& instantspeed) override;

    void readMainSpell(Cost& cost, Effect_H*& effect) override;

    void readSelector(Identifier& chars, Identifier& requs) override;
    void readModifier(char& i, Modifier& first_effect, Modifier*& other_effects) override;
    void readCosts(Cost& cost, bool& tapsymbol) override;
};

#endif //OLYMPUS_BINARYREADER_H
