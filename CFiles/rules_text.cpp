#include <utility>

#include ".header_link.h"
#include "../HFiles/8options.h"
#include "../HFiles/10triggers.h"
#include "../HFiles/head3_readfiles.h"

CardOracle::CardOracle(std::ifstream& myfile, std::string n, Mana c, char t):
    name(std::move(n)),
    cost(c), type(t),
    on_cast(nullptr),
    nb_actabs(0), first_actab(0),
    nb_triggers(0), typeof_triggers(0), triggers(0),
    flavor_text(0)
{ //stops after having read newline
	color = c.m2color();
	    //if(type == 1) color = 0; //lands are colorless but it doesn't matter right now

	check_safepoint(myfile, ' ', "before the rules text");
	check_safepoint(myfile, '{', "at the beginning of the rules text");
	char offset_text = (type==4) ? 2 : (type==3) ? 1 : 0; //2 for creatures, 1 for planeswalkers
	if(myfile.peek() == '}'){
		myfile.get();
	}
	else{
		enum section_types{onresolve, activated, triggered, flavor, astatic } section_name = activated;
		if(type == 0 || type == 5) section_name = onresolve;
		while(true){ //loop to read all sections
			god.gdebug(DBG_READFILE) << "Expecting section n°" << (int) section_name << std::endl;
			bool is_the_end = false;
			if(myfile.peek() != '<') switch(section_name){
				case onresolve: is_the_end = read_section_onresolve(myfile, on_cast); break;
				case activated: is_the_end = read_section_abils(myfile, nb_actabs, first_actab); break;
				case flavor: is_the_end = read_section_flavor(myfile, flavor_text, offset_text); break;
				case triggered: is_the_end = read_section_triggers(myfile, nb_triggers, typeof_triggers, triggers); break;
				case astatic: is_the_end = read_section_statics(myfile, nb_statics, statics); break;
 				//each stops right before '<' or after '}'
			}
			if(is_the_end) break;
			else{
			    check_safepoint(myfile, '<', "before section name");
                switch(myfile.get()){
                    case 'o': section_name = onresolve; break;
                    case 'a': section_name = activated; break;
                    case 'x': section_name = flavor; break;
                    case 't': section_name = triggered; break;
                    case 's': section_name = astatic; break;
                    default: raise_error("unrecognized section type (o, a, x, t, s)");
                }
                check_safepoint(myfile, '>', "after declaring another section");
			}
		}
	}
	if(offset_text != 0){
		if(!flavor_text){
			flavor_text = new char [offset_text+1];
			flavor_text[(int) offset_text] = 0;
		}
		if(type == 4){ //getting power and toughness
			int power, toughness;
			myfile >> power;
			check_safepoint(myfile, '/', "between power and toughness");
			myfile >> toughness;
			flavor_text[0] = (char) power;
			flavor_text[1] = (char) toughness;
		}
		if(type == 3){ //getting loyalty
			int loyalty;
			check_safepoint(myfile, ' ', "just before loyalty");
			check_safepoint(myfile, '\\', "just before loyalty number");
			myfile >> loyalty;
			flavor_text[0] = (char) loyalty;
			check_safepoint(myfile, '/', "just after loyalty number");
		}
	}
	check_safepoint(myfile, '\n', "after having given all necessary infos about the card");
	casted_id = generate_casted_id();
}

bool read_section_flavor(std::ifstream& myfile, char*& flavor_text, char offset_text){
    if(flavor_text != 0){
		raise_error("Flavor text of this spell declared multiple times");
	}
	int len = 0;
	char a;
	while((a = myfile.get())){
		len++;
		if(a == '<'){ a = 0; break; }
		else if(a == '}') break;
	}

    myfile.seekg(-len, std::ios_base::cur);
	if(a == 0) --len;
    flavor_text = new char [len + offset_text];
    myfile.get(flavor_text + offset_text, len);
    flavor_text[len + offset_text - 1] = '\0';
    god.gdebug(DBG_READFILE) << "Read full flavor: '" << flavor_text + offset_text << "'" << std::endl;
    if(myfile.get() == '}') return true;
    else return false;
}

bool read_section_abils(std::ifstream& myfile, int& nb_actabs, PermOption*& first_actab){
	if(nb_actabs != 0) raise_error("Ability text of this spell declared multiple times");
	char v = 0;
    nb_actabs = nb_phrases(myfile, v);
    if(nb_actabs == 0) return true;
    first_actab = new PermOption[nb_actabs];
    for(int i=0; i<nb_actabs; i++){
        bool tapsymbol = false, ismanaability = false;
        Mana cost = 0;
        PreResolvable* effects = 0;
        while(1){
            //reading ability cost
            char copy[30];
            int j = 0;
            do{
                copy[j++] = myfile.get();
                if(j==30) raise_error("Error: cost is longer than 30 symbols; are you sure you are declaring the cost of an activated ability?");
            } while(copy[j-1] != ',' && copy[j-1] != ':');//reading one cost
            god.gdebug(DBG_X_READFILE) << "Read" << copy << std::endl;
            if(copy[0] == 'T'){
                tapsymbol = true;
            }
            else{
                cost = Mana(copy);
                god.gdebug(DBG_X_READFILE) << copy << " was translated to " << cost << std::endl;
            }
            if(copy[j-1] == ':') break;
        }
        god.gdebug(DBG_READFILE) << "Tap symbol:" << tapsymbol <<", cost: " << cost.m2t() << std::endl;
        if(myfile.get() == ':') ismanaability = true; //reading either : or ' '
        read_section_onresolve(myfile, effects); //stops after reading '/'
        //the last stops before reading '<'
        first_actab[i].fillout(cost, effects, tapsymbol, ismanaability);
    }
	if(v == 2) return true;
	else return false;
}

bool read_section_triggers(std::ifstream& myfile, int& nb_triggers, char*& typeof_triggers, Trigger*& triggers){
    if(nb_triggers != 0) raise_error("Triggers of this spell declared multiple times");
    char v = 0;
    nb_triggers = nb_phrases(myfile, v);
    god.gdebug(DBG_READFILE) << "There are " << nb_triggers << " triggers" << std::endl;
    if(nb_triggers == 0) return false;

    triggers = new Trigger[nb_triggers];
    typeof_triggers = new char[nb_triggers];
    for(int i=0; i<nb_triggers; i++){
        char trigtype_tmp[20];
        myfile.get(trigtype_tmp, 20, ':');
        myfile.get(); //getting ':'
        myfile.get(); //getting ' ' or ':'
        int trigtype = olympus::dict_trigtypes.find(trigtype_tmp);
        if(trigtype == -1){
            raise_error(std::string("trigger ") + trigtype_tmp + " does not exist");
        }
        typeof_triggers[i] = (char) trigtype;
        PreResolvable* effects = 0;

        read_section_onresolve(myfile, effects); //stops after ' ' (or after })
        //the last stop after reading '<' or '}'
        triggers[i].setEffects(effects);
    }
	if(v == 2) return true;
	else return false;
}

/*
Most abilities are of the form do_something(target Blob, value x), like deal x damage to target Blob.
target can be 'target Thing (with characteristics)' [target (tapped) creature], 'all Things' (with characteristics), or 'you'.
An ability can also imply two targets 'target creature becomes a copy of another target creature', 'fights target creature', 'exchange life totals'
*/

bool read_section_onresolve(std::ifstream& myfile, PreResolvable*& preRes){ //reads up to ' ' or '}'
	if(preRes != 0) raise_error("OnResolve abilities of this spell declared multiple times");
	else{
		if(myfile.peek() == '}'){ myfile.get(); return true; }
		else{
			preRes = new PreResolvable();
			return preRes->init(myfile);
		}
	}
}

bool read_section_statics(std::ifstream& myfile, int& nbabs, StaticAb*& staticab){ //reads up to ending character: '<', '}' or '/'
//returns true if it is the end of rules text '}', and else false
    if(staticab != 0) raise_error("Static abilities of this spell declared multiple times");
    char v = 0;
    nbabs = nb_phrases(myfile, v);
    if(nbabs == 0) return true;

    staticab = new StaticAb[nbabs];
    for(int i=0; i<nbabs; i++){
        if(staticab->init(myfile)) v = 2; //starts at ( and ends at }
    }
    if(v == 2) return true;
    else return false;
}

bool PreResolvable::init(std::ifstream& myfile){ //reads from start of parameter (not ' ', which will be interpreted as void parameter!)
//up to the end character: ' ' or '}'
	bool is_the_end = false;
	char allassignedvariables[256] = {0};
	int nbassignedparams = 0;
	while(1){
		char p1 = read_ability_parameter(myfile, allassignedvariables, &nbassignedparams);
		int type = read_ability_type(myfile); //reads up to the space after the ability
		char p2 = read_ability_parameter(myfile, allassignedvariables, &nbassignedparams); //reads only the parameter itself
		
		fab = std::make_unique<Ability>(fab, p1, (char) type, p2);

        char a  = myfile.get();
        if(a == ',') myfile.get(); //we were at '.' in '{you draw 1.}' ; now we're at the symbol after, either '}' or ' '
        else if(a == '.'){
            char b = myfile.get();
            //god.gdebug(DBG_READFILE) << "Read " << b << std::endl;
            if(b == '}') is_the_end = true;
            break;
        }
        else raise_error("Expected '.' or ',' after an ability");
	}

	parameters = new char[nbassignedparams];
	nb_parameters = nbassignedparams;
	
	unsigned char i = 0;
	do{ //putting all parameters from the hashtable 'allassignedvariables' into the array 'parameters'
		if(allassignedvariables[(int) i] != 0){
			char value = (i&0xf0) + ((allassignedvariables[(int) i]&0xf0) >> 4);
			char index = allassignedvariables[(int) i]&0x0f;
			parameters[index - 1] = value;
		}
		i++;
	} while(i!=0);
	return is_the_end;
}

void read_staticeffect(std::ifstream& myfile, char& first_effect, char*& other_effects, char& nbeffects){ //read from ' '
	check_safepoint(myfile, ' ', "just after : of selector");
	char tmp[20]; int i = 0; nbeffects = 0;
	char tmp_effect[20];
	bool end_of_effects = false;
	while(!end_of_effects){
	    nbeffects++;
	    if(nbeffects > 20) raise_error("Can't handle more than 20 static effects");
        while(1){
            tmp[i++] = myfile.get();
            if(tmp[i-1] == '.' || tmp[i-1] == ','){ if(tmp[i-1] == '.') end_of_effects = true; tmp[i] = '\0'; break;}
            if(i >= 20) raise_error("wating for '.' just after static effect");
        }
        check_safepoint(myfile, ' ', "after . or , in statics");
        tmp_effect[nbeffects-1] = (char) olympus::dict_static_types.find(tmp);
        if(tmp_effect[nbeffects-1] == -1){
            raise_error(std::string("static ") + tmp + " does not exist");
        }
	}
    first_effect = tmp_effect[0];
	if(nbeffects == 1){
	    other_effects = 0;
	}
	else{
        other_effects = new char[nbeffects-1];
        for(i=1; i<nbeffects; i++){
            other_effects[i] = tmp_effect[i];
        }
	}
}

bool read_one_criterion(std::ifstream& myfile, Identifier& chars, Identifier& requs){ //reads up to ')' or ' '
    bool ret = false;
    char tmp[30];
    myfile.get(tmp, 30, '=');
    check_safepoint(myfile, '=', "when delaring selectors");
    int seltype = olympus::dict_selectors.find(tmp);
    if(seltype == -1) raise_error(std::string("selector type ") + tmp + " does not exist");
    int selcriteria, i = 0;
    while(1){
        tmp[i] = myfile.get();
        if(tmp[i] == ' ') break;
        else if(tmp[i] == ')'){ ret = true; tmp[i] = ' '; break; }
        ++i; if(i > 30) raise_error("Selector value longer than 30, are you sure you didn't forget something?");
    }
    switch(seltype){
        case 1:
            selcriteria = olympus::dict_selector_types.find(tmp);
            chars = chars | cid_objtype(selcriteria);
            requs = requs | rid_objtype;
            break;
        case 2:
            selcriteria = olympus::dict_selector_subtypes.find(tmp);
            chars = chars | cid_perm_type(selcriteria);
            requs = requs | rid_perm_type;
            break;
        case 3:
            selcriteria = olympus::dict_selector_tribes.find(tmp);
            chars = chars | cid_tribe(selcriteria);
            requs = requs | rid_tribe;
            break;
        case 4:
            selcriteria = olympus::dict_selector_tribes.find(tmp);
            chars = chars | cid_controller(selcriteria);
            requs = requs | rid_controller;
            break;
        default: raise_error("Ability implemented but does not have a dictionary");
    }
    if(selcriteria == -1) requs = ~(0); //unatteignable value (e.g. Cowards you control have)
    return ret;
}

//reads from '(' up to ':'
void read_selector(std::ifstream& myfile, Identifier& chars, Identifier& requs){
    chars = 0; requs = 0;
    check_safepoint(myfile, '(', "to declare a selector");
    if(read_one_criterion(myfile, chars, requs)){
        check_safepoint(myfile, ':', "after selector parentheses");
        return;
    } else {
        check_safepoint(myfile, '&', "between selectors");
        check_safepoint(myfile, ' ', "between selectors");
    }
}

int nb_phrases(std::ifstream& myfile, char& v){
    int ret = 0;
    long int startpos = myfile.tellg();
    while(v == 0){ //counting number of '.'-separated objects
        char a = myfile.get(); god.gdebug(DBG_READFILE) << a;
        switch(a){
            case '.': ret++; break;
            case '<': v = 1; break;
            case '}': v = 2; break;
        }
    }
    myfile.seekg(startpos, std::ios_base::beg); //we're going back one more character since we've read a first character before the if statement
    return ret;
}

bool StaticAb::init(std::ifstream& myfile){ //reads from '('
    //up to the end character: '<', '}' or '/'
    read_selector(myfile, _may, _must); //reads up to ':'
    read_staticeffect(myfile, first_effect, other_effects, nb_effects); //reads from ' ...' to '}' or '.'
    return (myfile.get()=='}');
}