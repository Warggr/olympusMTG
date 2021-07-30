#include <cstring>
#include "binarywriter.h"
#include "classes/1effects.h"
#include "classes/2triggers.h"
#include "classes/3statics.h"
#include "classes/4actabs.h"

void BinaryWriter::readName(std::string& name) {
    uint8_t length = name.length() - 1;
    directRead(length);
    ofile.write(name.c_str(), length);
}

void BinaryWriter::readAll(RulesHolder& rules, card_type type) {
    (void) type;
    char flags = 0;
    if(rules.first_actab != nullptr) flags += 0x1;
    if(rules.on_cast != nullptr) flags += 0x2;
    if(rules.statics != nullptr) flags += 0x4;
    if(rules.triggers != nullptr) flags += 0x8;
    if(rules.flavor_text != nullptr) flags += 0x10;
    if(flags & 0x1) readArray<ActAb_H>(rules.nb_actabs, rules.first_actab);
    if(flags & 0x2) read_section_onresolve(rules.on_cast);
    if(flags & 0x4) readArray<StaticAb_H>(rules.nb_statics, rules.statics);
    if(flags & 0x8) readArray<TriggerHolder_H>(rules.nb_triggers, rules.triggers);
    if(flags & 0x10) read_section_flavor(rules.flavor_text, 0); //offset_text is irrelevant here
}

void BinaryWriter::read_section_flavor(char *&flavor_text, uint8_t offset_text) {
    uint textsize = offset_text + strlen(flavor_text + offset_text);
    directRead<>(textsize);
    if(textsize != 0) {
        ofile.write(flavor_text, textsize);
    }
}

void BinaryWriter::readEffectH(uint8_t &nb_params, char *&params, std::forward_list<AtomEffect_H> &atoms) {
    canary('b');
    readNumberOfObjects(nb_params);
    uint nb_atoms = 0;
    for(auto& atom : atoms) nb_atoms++;
    readNumberOfObjects(nb_atoms);
    for(auto atom : atoms) atom.init(*this, params, nb_params);
    canary('y');
}

void BinaryWriter::readTriggerType(trigtype& type) {
    directRead<>(type);
}

void BinaryWriter::readAtomEffect(effect_type& type, flag_t*& params, uint8_t& effect_params, char* param_hashtable) {
    directRead(type);
    int nb_params = target_type::target_numbers[type];
    ofile.write(reinterpret_cast<char*>(params), nb_params);
}

void BinaryWriter::readActAb(Mana& mana, Effect_H*& effects, bool &tapsymbol, bool &ismanaability) {
    directRead(mana);
    effects->init(*this);
    char twobools = 0;
    if(tapsymbol) twobools += 0x1;
    if(ismanaability) twobools += 0x2;
    directRead(twobools);
}

void BinaryWriter::read_section_onresolve(Effect_H *&preRes) {
    preRes->init(*this);
}
