#ifndef OLYMPUS_BINARYWRITER_H
#define OLYMPUS_BINARYWRITER_H

#include "visitor.h"
#include <memory>

class BinaryWriter : public ReaderVisitor {
protected:
    virtual void canary(char canary) = 0;
    virtual void write(const char* chars, long size) = 0;
    virtual void savepoint() {};
public:
    void readName(std::string& name) override;
    void readManaCost(Mana& cost) override { directRead<>(cost); }
    void readCardType(card_type& type) override { directRead<>(type); }
    void read_section_flavor(char*& flavor_text, uint8_t offset_text) override;
    void read_section_othercasts(CardOptionListNode*& node) override;
    void readAll(RulesHolder& rules, card_type type) override;

    template<typename T>
    void directRead(T& value) {
        write(reinterpret_cast<char *>(&value), sizeof value);
    }

    void readNumberOfObjects(uint& nb) override { directRead<>(nb); }
    void readNumberOfObjects(uint8_t& nb) override { directRead<>(nb); }

    void readEffectH(uint8_t &nb_params, char *&params, std::forward_list<AtomEffect_H> &atoms) override;
    void readTriggerType(trigtype& type) override;
    void readAtomEffect(effect_type& type, flag_t*& params, uint8_t& nbparams, char* param_hashtable) override;
    void readActAb(Mana &mana, WeirdCost*& add_costs, Effect_H* effects, bool &tapsymbol, bool& ismanaability, bool& instantspeed) override;

    void readMainSpell(SpellOption& cast) override;
    void readSelector(Identifier &chars, Identifier &requs) override;
    void readModifier(char &i, Modifier &first_effect, Modifier *&other_effects) override;
    void readCosts(Mana &mana, bool &tapsymbol, WeirdCost *&others) override;
};

class BinaryFileWriter: public BinaryWriter {
    std::ostream& ofile;
    void canary(char canary) override {
        ofile.put(canary);
    }
    void write(const char* chars, long size) override {
        ofile.write(chars, size);
    }
    void raise_error(const std::string& msg) override { (void) msg; } //TODO
public:
    explicit BinaryFileWriter(std::ostream& ofile): ofile(ofile) {};
};

#endif //OLYMPUS_BINARYWRITER_H
