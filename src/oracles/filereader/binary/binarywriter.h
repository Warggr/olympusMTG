#ifndef OLYMPUS_BINARYWRITER_H
#define OLYMPUS_BINARYWRITER_H

#include "../writer.h"
#include "../visit.hpp"
#include <vector>
#include <memory>

class BinaryWriter : public WriterVisitor {
protected:
    void raiseError(const std::string& msg) override;
    template<typename T>
    void directWrite(const T& value) { ofile.write(reinterpret_cast<const char*>(&value), sizeof value); }
    template<typename T>
    void readArray(const std::vector<T>& array) {
        canary('a');
        ofile.put(static_cast<char>(array.size()));
        canary('b');
        for(const T& t : array) {
            ::visit<false>(t, *this);
        }
        canary('c');
    }
#ifdef F_CANARY
    void canary(char canary){ ofile.put(canary); }
#else
    void canary(char){};
#endif
public:
    BinaryWriter(std::ostream& output): WriterVisitor(output) {};
    void visit(const char* key, const std::string& str) override;
    void visit(const char* key, Mana mana) override;
    void visit(const char* key, bool b) override { canary(key[0]); directWrite(b); }
    void visit(const char* key, char value) override { canary(key[0]); directWrite(value); }
    void visit(const char* key, const card_type& type) override { canary(key[0]); directWrite(type); }
    void visit(const char* key, trig_type type) override { canary(key[0]); directWrite(type); }
    void visit(const char* key, const Cost& cost) override;

    void readEffect(const std::forward_list<AtomEffect_H> &effects, uint8_t nbparams, const char *param_hashtable) override;
    void readSectionFlavor(const char* flavor_text, uint8_t offset_text) override;
    void readSectionOthercasts(const fwdlist<CardOption>& node) override;
    void readAll(const RulesHolder& rules, const card_type& type) override;

/*    void readNumberOfObjects(uint& nb) override { directWrite<>(nb); }
    void readNumberOfObjects(uint8_t& nb) override { directWrite<>(nb); }*/

    void readAtomEffect(effect_type type, const flag_t* params) override;
    void readActAb(const Cost& cost, const Effect_H* effects, bool tapsymbol, bool ismanaability, bool instantspeed) override;

    void readMainSpell(const Cost& cost, const Effect_H* effect) override;
    void readSelector(Identifier chars, Identifier requs) override;
    void readModifier(char i, const Modifier& first_effect, const Modifier* other_effects) override;
    void readCosts(const Cost& costs, bool tapsymbol) override;
    void readSubtypes() override {};
};

#endif //OLYMPUS_BINARYWRITER_H
