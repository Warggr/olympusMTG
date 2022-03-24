#ifndef OLYMPUS_JSONREADER_H
#define OLYMPUS_JSONREADER_H

#include "../reader.h"

class JsonReader final : public ReaderVisitor {
    void canary(char){}
public:
    explicit JsonReader(std::istream& stream): ReaderVisitor(stream) {};

    void raiseError(const std::string& message) override;
    void readSectionFlavor(char*& flavor_text, uint8_t offset_text) override;
    void readSectionOthercasts(fwdlist<CardOption>& node) override;

    void visit(const char*, bool&) override {};
    void visit(const char*, char&) override {};
    void visit(const char*, std::string&) override {};
    void visit(const char*, card_type&) override {};
    void visit(const char*, trig_type&) override {};
    void visit(const char*, Cost&) override {};
    void visit(const char*, Mana&) override {};

    void readEffect(std::forward_list<AtomEffect_H>& effects, uint8_t& nbparams, char*& param_hashtable) override;

    void readAll(RulesHolder& rules, card_type type) override;

    void readActAb(Cost& cost, Effect_H* effects, bool &tapsymbol, bool &ismanaability, bool& instantspeed) override;
    void readSelector(Identifier& chars, Identifier& requs) override;
    void readModifier(char& i, Modifier& first_effect, Modifier*& other_effects) override;
    void readCosts(Cost& cost, bool& tapsymbol) override;
    void readSubtypes() override;

    void readMainSpell(Cost& cost, Effect_H*& effect) override;
};

#endif //OLYMPUS_JSONREADER_H
