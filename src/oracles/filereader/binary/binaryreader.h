#ifndef OLYMPUS_BINARYREADER_H
#define OLYMPUS_BINARYREADER_H

#include "../visitor.h"
#include <istream>

class BinaryReader : public ReaderVisitor {
    inline void canary(char canary){
        if(ifile.get() != canary) throw DeckbuildingError("Canary test failed");
    }
    std::istream& ifile;
public:
    explicit BinaryReader(std::istream& stream): ifile(stream) {};
    void raise_error(const std::string& message) override;
    void readName(std::string& name) override;
    void readCost(Cost& cost) override { directRead<>(cost); }
    void readCardType(card_type& type) override { directRead<>(type); }
    void read_section_flavor(char*& flavor_text, uint8_t offset_text) override;
    void read_section_othercasts(fwdlist<CardOption>& node) override;

    void readAll(RulesHolder& rules, card_type type) override;

    template<typename T>
    void directRead(T& value) {
        ifile.read(reinterpret_cast<char*>(&value), sizeof value);
    }

    void readNumberOfObjects(uint& nb) override { directRead<>(nb); }
    void readNumberOfObjects(uint8_t& nb) override { directRead<>(nb); }

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
