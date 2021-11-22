#include "jsonreader.h"
#include "oracles/classes/1effects.h"
#include "oracles/classes/2triggers.h"
#include "oracles/classes/3statics.h"
#include "oracles/classes/perm_option.h"

void JsonReader::readName(std::string& name) {
    uchar length = ifile.get();
    char* buffer = new char[length + 1];
    ifile.get(buffer, length + 1);
    buffer[(int)length] = '\0';
    name = std::string(buffer);
}

void JsonReader::readAll(RulesHolder& rules, card_type) {
    readMainSpell(rules.cost, rules.effects);
    char flags = ifile.get();
    if(flags & 0x1) readArray<PermOption>(rules.nb_actabs, rules.first_actab, false);
    if(flags & 0x2) read_section_othercasts(rules.otherCardOptions);
    if(flags & 0x4) readArray<StaticAb_H>(rules.nb_statics, rules.statics, false);
    if(flags & 0x8) readArray<TriggerHolder_H>(rules.nb_triggers, rules.triggers, false);
    if(flags & 0x10) read_section_flavor(rules.flavor_text, 0); //offset_text is irrelevant here
}

void JsonReader::read_section_flavor(char *&flavor_text, uint8_t) {
    uint textsize; directRead<>(textsize);
    if(textsize != 0) {
        flavor_text = new char[textsize];
        ifile.read(flavor_text, textsize);
    }
}

void JsonReader::readEffectH(uint8_t& nb_params, char *&params, std::forward_list<AtomEffect_H>& atoms) {
    canary('b');
    readNumberOfObjects(nb_params);
    if(nb_params) params = new char[nb_params];
    else params = nullptr;
    uint nb_atoms; readNumberOfObjects(nb_atoms);
    for(uint i=0; i<nb_atoms; i++) atoms.emplace_front(*this, params, nb_params);
    canary('y');
}

void JsonReader::readTriggerType(trig_type& type) {
    directRead<>(type);
}

void JsonReader::readAtomEffect(effect_type& type, flag_t*& params, uint8_t& effect_params, char* param_hashtable) {
    (void) effect_params; (void) param_hashtable;
    directRead(type);
    int nb_params = target_type::target_numbers[static_cast<int>(type)];
    params = new flag_t[nb_params];
    ifile.read(reinterpret_cast<char*>(params), nb_params);
}

void JsonReader::readActAb(Cost& cost, Effect_H* effects, bool& tapsymbol, bool& ismanaability, bool& instantspeed) {
    directRead(cost);
    effects->init(*this);
    char twobools = ifile.get();
    tapsymbol = twobools & 0x1; ismanaability = twobools & 0x2; instantspeed = twobools & 0x4;
}

void JsonReader::readMainSpell(Cost& cost, Effect_H*& effect) {
    (void) cost; (void) effect; //TODO
}

void JsonReader::readSelector(Identifier& chars, Identifier& requs) {
    (void) chars; (void) requs; //TODO
}

void JsonReader::readModifier(char& i, Modifier& first_effect, Modifier *&other_effects) {
    (void) i; (void) first_effect; (void) other_effects; //TODO
}

void JsonReader::readCosts(Cost& cost, bool& tapsymbol) {
    (void) cost; (void) tapsymbol; //TODO
}

void JsonReader::read_section_othercasts(fwdlist<CardOption>& node) {
    (void) node; //TODO
}

void JsonReader::raise_error(const std::string& message) {
    (void) message; //TODO raise error
}

void JsonReader::readNumberOfObjects(uint8_t& nb) { directRead<>(nb); }

void JsonReader::readNumberOfObjects(uint& nb) { directRead<>(nb); }

void JsonReader::readCardType(card_type& type) { directRead<>(type); }

void JsonReader::readCost(Cost& cost) { directRead<>(cost); }
