#include <cstring>
#include "binarywriter.h"
#include "oracles/classes/1effects.h"
#include "oracles/classes/2triggers.h"
#include "oracles/classes/3statics.h"
#include "oracles/classes/perm_option.h"

void BinaryWriter::readName(std::string& name) {
    uchar length = name.length() - 1;
    directRead(length);
    write(name.c_str(), length);
}

void BinaryWriter::readAll(RulesHolder& rules, card_type type) {
    (void) type;
    readMainSpell(rules.cast);
    char flags = 0;
    if(rules.first_actab) flags += 0x1;
    if(rules.otherCardOptions) flags += 0x2;
    if(rules.statics) flags += 0x4;
    if(rules.triggers) flags += 0x8;
    if(rules.flavor_text) flags += 0x10;
    if(flags & 0x1) readArray<PermOption>(rules.nb_actabs, rules.first_actab, true);
    if(flags & 0x2) read_section_othercasts(rules.otherCardOptions);
    if(flags & 0x4) readArray<StaticAb_H>(rules.nb_statics, rules.statics, true);
    if(flags & 0x8) readArray<TriggerHolder_H>(rules.nb_triggers, rules.triggers, true);
    if(flags & 0x10) read_section_flavor(rules.flavor_text, 0); //offset_text is irrelevant here
}

void BinaryWriter::read_section_flavor(char *&flavor_text, uint8_t offset_text) {
    uint textsize = offset_text + strlen(flavor_text + offset_text);
    directRead<>(textsize);
    if(textsize != 0) {
        write(flavor_text, textsize);
    }
}

void BinaryWriter::readEffectH(uint8_t &nb_params, char *&params, std::forward_list<AtomEffect_H> &atoms) {
    canary('b');
    readNumberOfObjects(nb_params);
    uint nb_atoms = std::distance(atoms.begin(), atoms.end());
    readNumberOfObjects(nb_atoms);
    for(auto atom : atoms) atom.init(*this, params, nb_params);
    canary('y');
}

void BinaryWriter::readTriggerType(trigtype& type) {
    directRead<>(type);
}

void BinaryWriter::readAtomEffect(effect_type& type, flag_t*& params, uint8_t&, char*) {
    directRead(type);
    int nb_params = target_type::target_numbers[static_cast<int>(type)];
    write(reinterpret_cast<char*>(params), nb_params);
}

void BinaryWriter::readActAb(Mana& mana, WeirdCost*& add_costs, Effect_H* effects,
                             bool& tapsymbol, bool& ismanaability, bool& instantspeed) {
    (void) add_costs;
    directRead(mana);
    effects->init(*this);
    char twobools = 0;
    if(tapsymbol) twobools += 0x1;
    if(ismanaability) twobools += 0x2;
    if(instantspeed) twobools += 0x4;
    directRead(twobools);
}

void BinaryWriter::read_section_othercasts(CardOptionListNode *&node) {
    (void) node; //TODO
}

void BinaryWriter::readMainSpell(SpellOption &cast) {
    (void) cast; //TODO
}

void BinaryWriter::readSelector(Identifier &chars, Identifier &requs) {
    (void) chars; (void) requs; //TODO
}

void BinaryWriter::readModifier(char &i, Modifier &first_effect, Modifier *&other_effects) {
    (void) i; (void) first_effect; (void) other_effects; //TODO
}

void BinaryWriter::readCosts(Mana &mana, bool &tapsymbol, WeirdCost *&others) {
    (void) mana; (void) tapsymbol; (void) others; //TODO
}
