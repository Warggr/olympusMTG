#ifndef OLYMPUS_BINARYWRITER_H
#define OLYMPUS_BINARYWRITER_H

#include "visitor.h"

class BinaryWriter : public ReaderVisitor {
    inline void canary(char canary){
        ofile.put(canary);
    }
    std::ostream ofile;
public:
    void readName(std::string& name) override;
    void readManaCost(Mana& cost) override { directRead<>(cost); }
    void readCardType(card_type& type) override { directRead<>(type); }
    void read_section_flavor(char*& flavor_text, uint8_t offset_text) override;

    void readAll(RulesHolder& rules, card_type type) override;

    template<typename T>
    T directRead() {
        T ret;
        ofile.write(reinterpret_cast<char *>(&ret), sizeof ret);
        return ret;
    }

    template<typename T>
    void directRead(T& value) {
        ofile.write(reinterpret_cast<char *>(&value), sizeof value);
    }

    void readNumberOfObjects(uint& nb) override { directRead<>(nb); }
    void readNumberOfObjects(uint8_t& nb) override { directRead<>(nb); }

    void readEffectH(uint8_t &nb_params, char *&params, std::forward_list<AtomEffect_H> &atoms) override;
    void readTriggerType(trigtype& type) override;
    void readAtomEffect(effect_type& type, flag_t*& params, uint8_t& nbparams, char* param_hashtable) override;
    void readActAb(Mana &mana, Effect_H *&effects, bool &tapsymbol, bool &ismanaability) override;

    void read_section_onresolve(Effect_H*& preRes) override;
};


#endif //OLYMPUS_BINARYWRITER_H
