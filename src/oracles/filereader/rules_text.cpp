#include "headI_identifiers.h"
#include "oracles/classes/5rulesholder.h"
#include "filereader.h"
#include "build_types.h"
#include "oracles/classes/1effects.h"
#include "oracles/classes/2triggers.h"
#include "oracles/classes/3statics.h"
#include "oracles/classes/perm_option.h"

void PlainFileReader::readAll(RulesHolder& rules, card_type type) {
	check_safepoint('{', "at the beginning of the rules text");
	uint8_t offset_text = defaultOffsetFor(type);
	if(ifile.peek() == '}'){
	    ifile.get();
	}
	else{
        check_safepoint(' ', "after opening {");
		enum section_types{ onresolve, altcosts, activated, triggered , flavor, astatic };
		section_types section_name = (type.underlying == card_type::sorcery) ? onresolve : activated;
		while(true){ //loop to read all sections
			gdebug(DBG_READFILE) << "Expecting section n°" << (int) section_name << "\n";
			if(ifile.peek() != '<') switch(section_name){
				case onresolve: readMainSpell(rules.cost, rules.effects); break;
				case activated: readArray<PermOption>(rules.nb_actabs, rules.first_actab, true); break;
				case flavor: read_section_flavor(rules.flavor_text, offset_text); break;
				case triggered: readArray<TriggerHolder_H>(rules.nb_triggers, rules.triggers, true); break;
				case astatic: readArray<StaticAb_H>(rules.nb_statics, rules.statics, true); break;
				case altcosts: read_section_othercasts(rules.otherCardOptions); break;
			}
			if(ifile.peek() == '}') break;
			else{
			    check_safepoint('<', "before section name");
                switch(ifile.get()){
                    case 'o': section_name = onresolve; break;
                    case 'a': section_name = activated; break;
                    case 'x': section_name = flavor; break;
                    case 't': section_name = triggered; break;
                    case 's': section_name = astatic; break;
                    case 'z': section_name = altcosts; break;
                    default: raise_error("unrecognized section type (o, a, x, t, s)"); return;
                }
                check_safepoint('>', "after declaring another section");
			}
		}
	}
    check_safepoint('}', "after rules text");
	if(offset_text != 0){
		if(!rules.flavor_text){
			rules.flavor_text = new char [offset_text+1];
			rules.flavor_text[(int) offset_text] = 0;
		}
		if(type.underlying == card_type::creature){ //getting power and toughness
			int power, toughness;
			ifile >> power;
			check_safepoint('/', "between power and toughness");
			ifile >> toughness;
			rules.flavor_text[0] = (char) power;
			rules.flavor_text[1] = (char) toughness;
		}
		else if(type.underlying == card_type::planeswalker){ //getting loyalty
			int loyalty;
			check_safepoint(' ', "just before loyalty");
			check_safepoint('\\', "just before loyalty number");
			ifile >> loyalty;
			rules.flavor_text[0] = (char) loyalty;
			check_safepoint('/', "just after loyalty number");
		}
	}
}

void PlainFileReader::read_section_flavor(char*& flavor_text, uint8_t offset_text){
    if(flavor_text != nullptr) raise_error("Flavor text of this spell declared multiple times");
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
    effects->init(*this);
}

void PlainFileReader::readTriggerType(trig_type& type){
    char trigtype_tmp[20];
    ifile.get(trigtype_tmp, 20, ':');
    ifile.get(); //getting ':'
    ifile.get(); //getting ' ' or ':'
    auto iter = dicts->dict_trigtypes.find(trigtype_tmp);
    if(iter == dicts->dict_trigtypes.not_found){
        raise_error(std::string("trigger ") + trigtype_tmp + " does not exist");
    }
    type = *iter;
}

void PlainFileReader::readEffectH(uint8_t &nb_params, char *&params, std::forward_list<AtomEffect_H>& atoms) {
	char allassignedvariables[256] = {0};
	uint8_t nbassignedparams = 0;
	while(state == go_on){
		atoms.emplace_front(*this, allassignedvariables, nbassignedparams);
	}
    state = go_on;

	params = new char[nbassignedparams];
	nb_params = nbassignedparams;

    //putting all parameters from the hashtable 'allassignedvariables' into the array 'parameters'
	for(uint i = 0; i < 256; i++) {
		if(allassignedvariables[i] != 0){
			char value = (i&0xf0) + ((allassignedvariables[i]&0xf0) >> 4);
			char index = allassignedvariables[i]&0x0f;
			params[index - 1] = value;
		}
	}
}

void PlainFileReader::readModifier(char& nbEffects, Modifier& firstEffect, Modifier*& otherEffects) {
	check_safepoint(' ', "just after : of selector");
	char tmp[20]; int i = 0; nbEffects = 0;
	Modifier::type tmp_effect[20];
	bool end_of_effects = false;
	while(!end_of_effects){
	    nbEffects++;
	    if(nbEffects > 20) raise_error("Can't handle more than 20 static effects");
        while(true){
            tmp[i++] = ifile.get();
            if(tmp[i-1] == '.' || tmp[i-1] == ','){ if(tmp[i-1] == '.') end_of_effects = true; tmp[i] = '\0'; break;}
            if(i >= 20) raise_error("waiting for '.' just after static effect");
        }
        check_safepoint(' ', "after . or , in statics");
        auto result = dicts->dict_static_types.find(tmp);
        if(result == dicts->dict_static_types.not_found)
            raise_error(std::string("static ") + tmp + " does not exist");
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
    check_safepoint('=', "when delaring selectors");
    auto seltype = dicts->dict_selectors.find(tmp);
    if(seltype == dicts->dict_selectors.not_found) raise_error(std::string("selector type ") + tmp + " does not exist");
    int i = 0;
    while(true){
        tmp[i] = ifile.get();
        if(tmp[i] == ' ') break;
        else if(tmp[i] == ')'){ ret = true; tmp[i] = ' '; break; }
        ++i; if(i > 30) raise_error("Selector value longer than 30, are you sure you didn't forget something?");
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
            raise_error("Ability implemented but does not have a dictionary");
    }
    if (!ret) consumeWhitespace();
    return ret;
}

//reads from '(' up to ':'
void PlainFileReader::read_selector(Identifier& chars, Identifier& requs){
    chars = 0; requs = 0;
    int nchars = 0, nrequs = 0;
    check_safepoint('(', "to declare a selector");
    while(true) {
        bool stopped = read_one_criterion(nchars, nrequs);
        chars = chars | nchars; requs = requs & nrequs; //TODO check for incompatible requirements
        if (stopped) {
            check_safepoint(':', "after selector parentheses");
            return;
        } else {
            check_safepoint('&', "between selectors");
            check_safepoint(' ', "between selectors");
        }
    }
}

uint PlainFileReader::nb_phrases(){
    uint ret = 0;
    long int startpos = ifile.tellg();
    while(true){ //counting number of 'sep'-separated objects
        char a = ifile.get(); gdebug(DBG_READFILE) << a;
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
    if(a != ';' && a != '.') raise_error(std::string("after atomic effect: expected ; or ., got ") + a);
    if(a == '.') state = breakout;
    check_safepoint(' ', "after ; or .");
}

void PlainFileReader::readSelector(Identifier &chars, Identifier &requs) {
    (void) chars; (void) requs; //TODO
}

void PlainFileReader::read_section_othercasts(fwdlist<CardOption>& node) {
    (void) node; //TODO
}

void PlainFileReader::readMainSpell(Cost& cost, Effect_H*& effects) {
    (void) cost; (void) effects; //TODO
}
