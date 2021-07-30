#ifndef OLYMPUS_BINARYREADER_H
#define OLYMPUS_BINARYREADER_H

#include "visitor.h"

class BinaryReader : public ReaderVisitor {
    inline void canary(char canary){
        if(ifile.get() != canary) throw DeckbuildingError("Canary test failed");
    }
    std::istream ifile;
public:
    void readName(std::string& name) override;
    void readManaCost(Mana& cost) override { directRead<>(cost); }
    void readCardType(card_type& type) override { directRead<>(type); }
    void read_section_flavor(char*& flavor_text, uint8_t offset_text) override;

    void readAll(RulesHolder& rules, card_type type) override;

    template<typename T>
    T directRead() {
        T ret;
        ifile.read(reinterpret_cast<char *>(&ret), sizeof ret);
        return ret;
    }

    template<typename T>
    void directRead(T& value) {
        ifile.read(reinterpret_cast<char *>(&value), sizeof value);
    }

    void readNumberOfObjects(uint& nb) override { directRead<>(nb); }
    void readNumberOfObjects(uint8_t& nb) override { directRead<>(nb); }

    void readEffectH(uint8_t &nb_params, char *&params, std::forward_list<AtomEffect_H> &atoms) override;
    void readTriggerType(trigtype& type) override;
    void readAtomEffect(effect_type& type, flag_t*& params, uint8_t& nbparams, char* param_hashtable) override;
    void readActAb(Mana &mana, Effect_H *&effects, bool &tapsymbol, bool &ismanaability) override;

    void read_section_onresolve(Effect_H*& preRes) override {
        if(preRes != nullptr) raise_error("OnResolve abilities of this spell declared multiple times");
        preRes = new Effect_H(*this);
    }
};

#endif //OLYMPUS_BINARYREADER_H
