#include "headI_identifiers.h"
#include "classes/5rulesholder.h"
#include "filereader.h"
#include "oracles/filereader/plain/Dictionary/dictholder.h"
#include "build_types.h"
#include "classes/1effects.h"
#include "classes/2triggers.h"
#include "classes/3statics.h"
#include "classes/perm_option.h"

void PlainFileReader::readAll(RulesHolder& rules, card_type type) {
    checkSafepoint('{', "at the beginning of the rules text");
    uint8_t offset_text = defaultOffsetFor(type);
    if(ifile.peek() == '}'){
        ifile.get();
    } else {
        checkSafepoint(' ', "after opening {");
        enum section_types{ onresolve, altcosts, activated, triggered , flavor, astatic };
        section_types section_name = (type.underlying == card_type::sorcery) ? onresolve : activated;
        while(true){ //loop to read all sections
            gdebug(DBG_READFILE) << "Expecting section n°" << (int) section_name << "\n";
            if(ifile.peek() != '<') switch(section_name){
                case onresolve: readMainSpell(rules.cost, rules.effects); break;
                case activated: readArray<>(rules.actabs); break;
                case flavor: readSectionFlavor(rules.flavor_text, offset_text); break;
                case triggered: readArray<TriggerHolder_H>(rules.triggers); break;
                case astatic: readArray<StaticAb_H>(rules.statics); break;
                case altcosts: readSectionOthercasts(rules.otherCardOptions); break;
            }
            if(ifile.peek() == '}') break;
            else{
                checkSafepoint('<', "before section name");
                switch(ifile.get()){
                    case 'o': section_name = onresolve; break;
                    case 'a': section_name = activated; break;
                    case 'x': section_name = flavor; break;
                    case 't': section_name = triggered; break;
                    case 's': section_name = astatic; break;
                    case 'z': section_name = altcosts; break;
                    default: raiseError("unrecognized section type (o, a, x, t, s)"); return;
                }
                checkSafepoint('>', "after declaring another section");
            }
        }
    }
    checkSafepoint('}', "after rules text");
    if(offset_text != 0){
        if(!rules.flavor_text){
            rules.flavor_text = new char [offset_text+1];
            rules.flavor_text[(int) offset_text] = 0;
        }
        if(type.underlying == card_type::creature){ //getting power and toughness
            int power, toughness;
            ifile >> power;
            checkSafepoint('/', "between power and toughness");
            ifile >> toughness;
            rules.flavor_text[0] = (char) power;
            rules.flavor_text[1] = (char) toughness;
        }
        else if(type.underlying == card_type::planeswalker){ //getting loyalty
            int loyalty;
            checkSafepoint(' ', "just before loyalty");
            checkSafepoint('\\', "just before loyalty number");
            ifile >> loyalty;
            rules.flavor_text[0] = (char) loyalty;
            checkSafepoint('/', "just after loyalty number");
        }
    }
}

void PlainFileReader::readSectionFlavor(char*& flavor_text, uint8_t offset_text){
    if(flavor_text != nullptr) raiseError("Flavor text of this spell declared multiple times");
    int len = 0;
    char a;
    while((a = ifile.get())){
        len++;
        if(a == '<'){ a = 0; state = breakout; break; }
        else if(a == '}') { state = end_reached; break; }
    }

    ifile.seekg(-len, std::ios_base::cur);
    if(a == 0) --len;
    flavor_text = new char [len + offset_text];
    ifile.get(flavor_text + offset_text, len);
    flavor_text[len + offset_text - 1] = '\0';
    gdebug(DBG_READFILE) << "Read full flavor: '" << flavor_text + offset_text << "'\n";
}

void PlainFileReader::readActAb(Cost& cost, Effect_H* effects,
                                bool& tapsymbol, bool& ismanaability, bool& instantspeed) {
    //assert(effects != nullptr);
    readCosts(cost, tapsymbol);
    instantspeed = true; //Unless specified, actabs are instant-speed

    //gdebug(DBG_READFILE) << "Tap symbol:" << tapsymbol <<", cost: " << mana.m2t() << "\n";
    ismanaability = (ifile.get() == ':'); //reading either : or ' '
    ::visit<true>(*effects, *this);
}

void PlainFileReader::visit(const char*, trig_type& type){
    char trigtype_tmp[20];
    ifile.get(trigtype_tmp, 20, ':');
    ifile.get(); //getting ':'
    ifile.get(); //getting ' ' or ':'
    auto iter = dicts->dict_trigtypes.find(trigtype_tmp);
    if(iter == dicts->dict_trigtypes.not_found){
        raiseError(std::string("trigger ") + trigtype_tmp + " does not exist");
    }
    type = *iter;
}

void PlainFileReader::readEffect(std::forward_list<AtomEffect_H>& effects, uint8_t& nbparams, char*& param_hashtable) {
    char allassignedvariables[256] = {0};
    uint8_t nbassignedparams = 0;
    while(state == go_on){ //TODO OPTI optimize this
        effect_type type; flag_t* params = nullptr;
        readAtomEffect(type, params, nbassignedparams, allassignedvariables);
        effects.emplace_front(type, params);
    }
    state = go_on;

    param_hashtable = new char[nbassignedparams];
    nbparams = nbassignedparams;

    //putting all parameters from the hashtable 'allassignedvariables' into the array 'parameters'
    for(uint i = 0; i < 256; i++) {
        if(allassignedvariables[i] != 0){
            char value = (i&0xf0) + ((allassignedvariables[i]&0xf0) >> 4);
            char index = allassignedvariables[i]&0x0f;
            param_hashtable[index - 1] = value;
        }
    }
}

void PlainFileReader::readModifier(char& nbEffects, Modifier& firstEffect, Modifier*& otherEffects) {
    checkSafepoint(' ', "just after : of selector");
    char tmp[20]; int i = 0; nbEffects = 0;
    Modifier::type tmp_effect[20];
    bool end_of_effects = false;
    while(!end_of_effects){
        nbEffects++;
        if(nbEffects > 20) raiseError("Can't handle more than 20 static effects");
        while(true){
            tmp[i++] = ifile.get();
            if(tmp[i-1] == '.' || tmp[i-1] == ','){ if(tmp[i-1] == '.') end_of_effects = true; tmp[i] = '\0'; break;}
            if(i >= 20) raiseError("waiting for '.' just after static effect");
        }
        checkSafepoint(' ', "after . or , in statics");
        auto result = dicts->dict_static_types.find(tmp);
        if(result == dicts->dict_static_types.not_found)
            raiseError(std::string("static ") + tmp + " does not exist");
        else
            tmp_effect[i] = *result;
    }
    firstEffect.myType = tmp_effect[0];
    if(nbEffects == 1) otherEffects = nullptr;
    else{
        otherEffects = new Modifier[nbEffects - 1];
        for(i=1; i<nbEffects; i++){
            otherEffects[i].myType = tmp_effect[i];
        }
    }
}

bool PlainFileReader::read_one_criterion(Identifier& chars, Identifier& requs){ //reads up to ')' or ' '
    bool ret = false;
    char tmp[30];
    ifile.get(tmp, 30, '=');
    checkSafepoint('=', "when delaring selectors");
    auto seltype = dicts->dict_selectors.find(tmp);
    if(seltype == dicts->dict_selectors.not_found) raiseError(std::string("selector type ") + tmp + " does not exist");
    int i = 0;
    while(true){
        tmp[i] = ifile.get();
        if(tmp[i] == ' ') break;
        else if(tmp[i] == ')'){ ret = true; tmp[i] = ' '; break; }
        ++i; if(i > 30) raiseError("Selector value longer than 30, are you sure you didn't forget something?");
    }
    switch (*seltype) {
        case types: {
            auto selcriteria = dicts->dict_selector_types.find(tmp);
            if (selcriteria == dicts->dict_selector_types.not_found)
                requs = ~(0); //unatteignable value (e.g. Cowards you control have)
            chars = chars | cid_objtype(*selcriteria);
            requs = requs | rid_objtype;
            break; }
        case subtypes: {
            auto selcriteria = dicts->dict_selector_subtypes.find(tmp);
            if (selcriteria == dicts->dict_selector_subtypes.not_found)
                requs = ~(0); //unatteignable value (e.g. Cowards you control have)
            chars = chars | cid_perm_type((permanent_type) *selcriteria);
            requs = requs | rid_perm_type;
            break; }
        case tribes: {
            auto selcriteria = dicts->dict_selector_tribes.find(tmp);
            if (selcriteria == dicts->dict_selector_tribes.not_found)
                requs = ~(0); //unatteignable value (e.g. Cowards you control have)
            chars = chars | cid_tribe(*selcriteria);
            requs = requs | rid_tribe;
            break; }
        case players: {
            auto selcriteria = dicts->dict_selector_players.find(tmp);
            if (selcriteria == dicts->dict_selector_players.not_found)
                requs = ~(0); //unatteignable value (e.g. Cowards you control have)
            chars = chars | cid_controller(*selcriteria);
            requs = requs | rid_controller;
            break; }
        default:
            raiseError("Ability implemented but does not have a dictionary");
    }
    if (!ret) consumeWhitespace();
    return ret;
}

//reads from '(' up to ':'
void PlainFileReader::readSelector(Identifier& chars, Identifier& requs){
    chars = 0; requs = 0;
    int nchars = 0, nrequs = 0;
    checkSafepoint('(', "to declare a selector");
    while(true) {
        bool stopped = read_one_criterion(nchars, nrequs);
        chars = chars | nchars; requs = requs & nrequs; //TODO check for incompatible requirements
        if (stopped) {
            checkSafepoint(':', "after selector parentheses");
            return;
        } else {
            checkSafepoint('&', "between selectors");
            checkSafepoint(' ', "between selectors");
        }
    }
}

uint PlainFileReader::nb_phrases(){
    uint ret = 0;
    long int startpos = ifile.tellg();
    while(true){ //counting number of 'sep'-separated objects
        char a = ifile.get();
        if(a == '<' || a == '}') break;
        if(a == '.') ret++;
    }
    ifile.seekg(startpos, std::ios_base::beg); //we're going back one more character since we've read a first character before the if statement
    return ret;
}

void PlainFileReader::readAtomEffect(effect_type& type, flag_t*& params, uint8_t& effect_params, char* param_hashtable) {
    type = readAbilityType();
    int nb_params = target_type::target_numbers[(int)type];
    params = new flag_t [nb_params];

    for(int i=0; i<nb_params; i++){
        params[i] = readAbilityParameter(param_hashtable, effect_params, target_type::target_types[(int)type][i]);
    }
    char a = ifile.get();
    if(a != ';' && a != '.') raiseError(std::string("after atomic effect: expected ; or ., got ") + a);
    if(a == '.') state = breakout;
    checkSafepoint(' ', "after ; or .");
}

void PlainFileReader::readSectionOthercasts(fwdlist<CardOption>& node) {
    (void) node; //TODO
}

void PlainFileReader::readMainSpell(Cost& cost, Effect_H*& effects) {
    (void) cost; (void) effects; //TODO
}
