#include <cstring>
#include "jsonwriter.h"
#include "classes/1effects.h"
#include "classes/2triggers.h"
#include "classes/3statics.h"
#include "classes/5rulesholder.h"
#include "classes/perm_option.h"

void JsonWriter::readAll(RulesHolder& rules, card_type) {
    readMainSpell(rules.cost, rules.effects);
    if(rules.first_actab) readArray<PermOption>(rules.nb_actabs, rules.first_actab);
    if(!rules.otherCardOptions.empty()) readSectionOthercasts(rules.otherCardOptions);
    if(rules.statics) readArray<StaticAb_H>(rules.nb_statics, rules.statics);
    if(rules.statics) readArray<TriggerHolder_H>(rules.nb_triggers, rules.triggers);
    if(rules.flavor_text) readSectionFlavor(rules.flavor_text, 0); //offset_text is irrelevant here
}

void JsonWriter::readSectionFlavor(char*& flavor_text, uint8_t offset_text) {
    uint textsize = offset_text + strlen(flavor_text) + offset_text;
    directRead<>(textsize);
    if(textsize != 0) {
        write(flavor_text, textsize);
    }
}

void JsonWriter::readEffectH(uint8_t& nb_params, char *&params, std::forward_list<AtomEffect_H>& atoms) {
    readNumberOfObjects(nb_params);
    uint nb_atoms = std::distance(atoms.begin(), atoms.end());
    readNumberOfObjects(nb_atoms);
    for(auto atom : atoms) atom.init(*this, params, nb_params);
}

void JsonWriter::readAtomEffect(effect_type& type, flag_t*& params, uint8_t&, char*) {
    _visit("type", type);
    int nb_params = target_type::target_numbers[static_cast<int>(type)];
    write(reinterpret_cast<char*>(params), nb_params);
}

void JsonWriter::readActAb(Cost& cost, Effect_H* effects,
                             bool& tapsymbol, bool& ismanaability, bool& instantspeed) {
    visit("cost", cost);
    visit("effects", effects);
    _visit("tap symbol", tapsymbol);
    _visit("mana ability", ismanaability);
    _visit("instantspeed", instantspeed);
}

void JsonWriter::readSectionOthercasts(fwdlist<CardOption>& node) {
    (void) node; //TODO
}

void JsonWriter::readSelector(Identifier& chars, Identifier& requs) {
    visit("chars", chars);
    visit("requs", requs);
}

void JsonWriter::readModifier(char& i, Modifier& first_effect, Modifier *&other_effects) {
    (void) i; (void) first_effect; (void) other_effects; //TODO
}

void JsonWriter::readCosts(Cost& cost, bool& tapsymbol) {
    (void) cost; (void) tapsymbol; //TODO
}

void JsonWriter::readMainSpell(Cost& cost, Effect_H*& effect) {
    (void) cost; (void) effect; //TODO
}

void JsonWriter::visit(const char* key, const Effect_H* effect) {
    writeKey(key);
    startObj();
    effect->write(*this);
    endObj();
    comma = true;
}

void JsonWriter::write(trig_type type) {
    const char* descriptions[] = { "etb", "ltb", "statechg", "become", "atk", "dealtdmg"};
    ofile << descriptions[static_cast<int>(type)];
}

void JsonWriter::write(const card_type& type) {
    startObj();
    visit("legendary", static_cast<bool>(type.legendary));

    endObj();
}

void JsonWriter::startObj() {
    ofile.put('{');
    indent++;
    if(pretty) {
        ofile.put('\n');
        for(uint i = 0; i < indent; i++) ofile.put('\t');
    }
    comma = false;
}

void JsonWriter::endObj() {
    indent--;
    if(pretty){
        ofile.put('\n');
        for (uint i = 0; i<indent; i++) ofile.put('\t');
    }
    ofile.put('}');
}
