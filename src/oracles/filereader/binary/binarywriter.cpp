#include <cstring>
#include "binarywriter.h"
#include "classes/1effects.h"
#include "classes/2triggers.h"
#include "classes/3statics.h"
#include "classes/perm_option.h"
#include "classes/5rulesholder.h"
#include "../visit.hpp"

void BinaryWriter::visit(const char* key, const std::string& str) {
    canary(key[0]);

    uchar length = str.length();
    directWrite(length);
    ofile.write(str.c_str(), str.length());
}

void BinaryWriter::readAll(const RulesHolder& rules, const card_type&) {
    readMainSpell(rules.cost, rules.effects);
    char flags = 0;
    if(!rules.actabs.empty()) flags += 0x1;
    if(!rules.otherCardOptions.empty()) flags += 0x2;
    if(!rules.statics.empty()) flags += 0x4;
    if(!rules.triggers.empty()) flags += 0x8;
    if(rules.flavor_text) flags += 0x10;
    ofile << flags;
    if(flags & 0x1) readArray<>(rules.actabs);
    if(flags & 0x2) readSectionOthercasts(rules.otherCardOptions);
    if(flags & 0x4) readArray<StaticAb_H>(rules.statics);
    if(flags & 0x8) readArray<TriggerHolder_H>(rules.triggers);
    if(flags & 0x10) readSectionFlavor(rules.flavor_text, 0); //offset_text is irrelevant here
}

void BinaryWriter::readSectionFlavor(const char* flavor_text, uint8_t offset_text) {
    uint textsize = offset_text + strlen(flavor_text + offset_text);
    directWrite<>(textsize);
    if(textsize != 0) {
        ofile.write(flavor_text, textsize);
    }
}

/*void BinaryWriter::readEffectH(uint8_t& nb_params, char *&params, std::forward_list<AtomEffect_H>& atoms) {
    canary('b');
    readNumberOfObjects(nb_params);
    uint nb_atoms = std::distance(atoms.begin(), atoms.end());
    readNumberOfObjects(nb_atoms);
    for(auto atom : atoms) atom.init(*this, params, nb_params);
    canary('y');
}*/

void BinaryWriter::readAtomEffect(effect_type type, const flag_t* params) {
    directWrite(type);
    int nb_params = target_type::target_numbers[static_cast<int>(type)];
    ofile.write(reinterpret_cast<const char*>(params), nb_params);
}

void BinaryWriter::readActAb(const Cost& cost, const Effect_H* effects,
                             bool tapsymbol, bool ismanaability, bool instantspeed) {
    directWrite(cost);
    ::visit<false>(*effects, *this);
    char twobools = 0;
    if(tapsymbol) twobools += 0x1;
    if(ismanaability) twobools += 0x2;
    if(instantspeed) twobools += 0x4;
    directWrite(twobools);
}

void BinaryWriter::readSectionOthercasts(const fwdlist<CardOption>& node) {
    (void) node; //TODO
}

void BinaryWriter::readSelector(Identifier chars, Identifier requs) {
    (void) chars; (void) requs; //TODO
}

void BinaryWriter::readModifier(char i, const Modifier& first_effect, const Modifier* other_effects) {
    (void) i; (void) first_effect; (void) other_effects; //TODO
}

void BinaryWriter::readCosts(const Cost& cost, bool tapsymbol) {
    (void) cost; (void) tapsymbol; //TODO
}

void BinaryWriter::readMainSpell(const Cost& cost, const Effect_H* effect) {
    (void) cost; (void) effect; //TODO
}

void BinaryWriter::visit(const char* key, const Cost& cost) {
    canary(key[0]);
    directWrite<>(cost);
}

void BinaryWriter::visit(const char* key, Mana mana) { canary(key[0]); directWrite(mana); }

void BinaryWriter::readEffect(const std::forward_list<AtomEffect_H>& effects, uint8_t nbparams, const char* param_hashtable) {
    directWrite(nbparams);
    ofile.write(param_hashtable, nbparams);

    unsigned char x = std::distance(effects.begin(), effects.end());
    directWrite(x);
    for(auto& eff : effects) {
        readAtomEffect(eff.getType(), eff.getParams());
    }
}

void BinaryWriter::raiseError(const std::string& msg) {
    (void) msg; //TODO
}
