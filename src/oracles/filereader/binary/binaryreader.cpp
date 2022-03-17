#include "binaryreader.h"
#include "../visit.hpp"
#include "classes/1effects.h"
#include "classes/2triggers.h"
#include "classes/3statics.h"
#include "classes/perm_option.h"
#include "classes/5rulesholder.h"
#include <exception>

void BinaryReader::visit(const char* key, std::string& name) {
    canary(key[0]);

    uchar length = ifile.get();
    char* buffer = new char[length + 1];
    ifile.read(buffer, length + 1);
    buffer[(int)length] = '\0';
    name = std::string(buffer);
}

void BinaryReader::readAll(RulesHolder& rules, card_type) {
    readMainSpell(rules.cost, rules.effects);
    char flags = ifile.get();
    if(flags & 0x1) readArray<PermOption>(rules.nb_actabs, rules.first_actab);
    if(flags & 0x2) readSectionOthercasts(rules.otherCardOptions);
    if(flags & 0x4) readArray<StaticAb_H>(rules.nb_statics, rules.statics);
    if(flags & 0x8) readArray<TriggerHolder_H>(rules.nb_triggers, rules.triggers);
    if(flags & 0x10) readSectionFlavor(rules.flavor_text, 0); //offset_text is irrelevant here
}

void BinaryReader::readSectionFlavor(char*& flavor_text, uint8_t) {
    uint textsize; directRead<>(textsize);
    if(textsize != 0) {
        flavor_text = new char[textsize];
        ifile.read(flavor_text, textsize);
    }
}

/*void BinaryReader::readEffectH(uint8_t& nb_params, char*& params, std::forward_list<AtomEffect_H>& atoms) {
    canary('b');
    readNumberOfObjects(nb_params);
    if(nb_params) params = new char[nb_params];
    else params = nullptr;
    uint nb_atoms; readNumberOfObjects(nb_atoms);
    for(uint i=0; i<nb_atoms; i++) atoms.emplace_front(*this, params, nb_params);
    canary('y');
}*/

void BinaryReader::readAtomEffect(effect_type& type, flag_t*& params) {
    directRead(type);
    int nb_params = target_type::target_numbers[static_cast<int>(type)];
    params = new flag_t[nb_params];
    ifile.read(reinterpret_cast<char*>(params), nb_params);
}

void BinaryReader::readActAb(Cost& cost, Effect_H* effects, bool& tapsymbol, bool& ismanaability, bool& instantspeed) {
    directRead(cost);
    ::visit<true>(*effects, *this);
    char twobools = ifile.get();
    tapsymbol = twobools & 0x1; ismanaability = twobools & 0x2; instantspeed = twobools & 0x4;
}

void BinaryReader::readMainSpell(Cost& cost, Effect_H*& effect) {
    (void) cost; (void) effect; //TODO
}

void BinaryReader::readSelector(Identifier& chars, Identifier& requs) {
    (void) chars; (void) requs; //TODO
}

void BinaryReader::readModifier(char& i, Modifier& first_effect, Modifier*& other_effects) {
    (void) i; (void) first_effect; (void) other_effects; //TODO
}

void BinaryReader::readCosts(Cost& cost, bool& tapsymbol) {
    (void) cost; (void) tapsymbol; //TODO
}

void BinaryReader::readSectionOthercasts(fwdlist<CardOption>& node) {
    (void) node; //TODO
}

void BinaryReader::raiseError(const std::string& message) {
    (void) message;
    throw std::exception(); //TODO FEATURE print the message somewhere
}

void BinaryReader::visit(const char* key, Cost& cost) {
    canary(key[0]);
    directRead<>(cost);
}

/*void BinaryReader::visit(const char*, Effect_H*& effect) {
    effect = new Effect_H(*this);
}*/

void BinaryReader::readEffect(std::forward_list<AtomEffect_H>& effects, uint8_t& nbparams, char*& param_hashtable) {
    nbparams = ifile.get();
    param_hashtable = new char[nbparams];
    ifile.read(param_hashtable, nbparams);

    unsigned char nb_effects; directRead(nb_effects);
    for(int i = 0; i<nb_effects; i++) {
        effect_type type; flag_t* params; //TODO OPTI optimize
        readAtomEffect(type, params);
        effects.emplace_front(type, params);
    }
}
