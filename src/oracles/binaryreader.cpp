#include "binaryreader.h"
#include "classes/1effects.h"
#include "classes/2triggers.h"
#include "classes/3statics.h"
#include "classes/4actabs.h"

void BinaryReader::readName(std::string& name) {
    char length = ifile.get();
    char* buffer = new char[length + 1];
    ifile.get(buffer, length + 1);
    buffer[length] = '\0';
    name = std::string(buffer);
}

void BinaryReader::readAll(RulesHolder& rules, card_type type) {
    (void) type;
    char flags = ifile.get();
    if(flags & 0x1) readArray<ActAb_H>(rules.nb_actabs, rules.first_actab);
    if(flags & 0x2) read_section_onresolve(rules.on_cast);
    if(flags & 0x4) readArray<StaticAb_H>(rules.nb_statics, rules.statics);
    if(flags & 0x8) readArray<TriggerHolder_H>(rules.nb_triggers, rules.triggers);
    if(flags & 0x10) read_section_flavor(rules.flavor_text, 0); //offset_text is irrelevant here
}

void BinaryReader::read_section_flavor(char *&flavor_text, uint8_t offset_text) {
    uint textsize; directRead<>(textsize);
    if(textsize != 0) {
        flavor_text = new char[textsize];
        ifile.read(flavor_text, textsize);
    }
}

void BinaryReader::readEffectH(uint8_t &nb_params, char *&params, std::forward_list<AtomEffect_H> &atoms) {
    canary('b');
    readNumberOfObjects(nb_params);
    if(nb_params) params = new char[nb_params];
    else params = nullptr;
    uint nb_atoms; readNumberOfObjects(nb_atoms);
    for(int i=0; i<nb_atoms; i++) atoms.emplace_front(*this, params, nb_params);
    canary('y');
}

void BinaryReader::readTriggerType(trigtype& type) {
    directRead<>(type);
}

void BinaryReader::readAtomEffect(effect_type& type, flag_t*& params, uint8_t& effect_params, char* param_hashtable) {
    directRead(type);
    int nb_params = target_type::target_numbers[type];
    params = new flag_t[nb_params];
    ifile.read(reinterpret_cast<char*>(params), nb_params);
}

void BinaryReader::readActAb(Mana& mana, Effect_H*& effects, bool &tapsymbol, bool &ismanaability) {
    directRead(mana);
    effects = new Effect_H(*this);
    char twobools = ifile.get();
    tapsymbol = twobools & 0x1; ismanaability = twobools & 0x2;
}
