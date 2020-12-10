#include "../HFiles/olympus_main.h"

CardOracle::CardOracle(std::ifstream& myfile, std::string n, Mana c, char t): name(n), cost(c), type(t), on_cast(0), nb_actabs(0), first_actab(0), nb_triggers(0), typeof_triggers(0), triggers(0), flavor_text(0){ //stops after having read newline
	color = mana2color(c);
	//if(type == 1) color = 0; //lands are colorless but it doesn't matter right now

	check_safepoint(myfile, ' ', "before the rules text");
	check_safepoint(myfile, '{', "at the beginning of the rules text");
	int offset_text = (type==4) ? 2 : (type==3) ? 1 : 0; //2 for creatures, 1 for planeswalkers
	if(myfile.peek() == '}'){
		flavor_text = 0;
		myfile.get();
	}
	else{
		char section_name = 1; //1 for 'ability text'
		if(type == 0 || type == 5) section_name = 0; //0 for 'oncast/on_etb text'
		while(1){ //loop to read all sections
			god.gdebug(DBG_READFILE) << "Expecting section n°" << section_name << std::endl;
			bool is_the_end;
			switch(section_name){
				case 0: is_the_end = read_section_onresolve(myfile, &on_cast); break;
				case 1: is_the_end = read_section_abils(myfile, &nb_actabs, &first_actab); break;
				case 2: is_the_end = read_section_flavor(myfile, &flavor_text, offset_text); break;
				case 3: is_the_end = read_section_triggers(myfile, &nb_triggers, &typeof_triggers, &triggers); break;
 				//each stops after reading '<' or '}'
			}
			if(is_the_end) break;
			else{
				switch(myfile.get()){
					case 'o': section_name = 0; break;
					case 'a': section_name = 1; break;
					case 'x': section_name = 2; break;
					case 't': section_name = 3; break;
				}
				check_safepoint(myfile, '>', "after declaring another section");
			}
		}
	}
	if(offset_text != 0){
		if(!flavor_text){
			flavor_text = new char [offset_text+1];
			flavor_text[offset_text] = 0;
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
	else check_safepoint(myfile, '\n', "after having given all necessary infos about the card");
}

bool read_section_flavor(std::ifstream& myfile, char** flavor_text, char offset_text){
	if(*flavor_text != 0){
		raise_error("Flavor text of this spell declared multiple times");
	}
	int len = 0;
	char a;
	while((a = myfile.get())){
		len++;
		if(a=='<' || a == '}') break;
	}
	if(len == 1){
		*flavor_text = 0;
		return false;
	}
	else{
		myfile.seekg(-len, std::ios_base::cur);
		*flavor_text = new char [len + offset_text];
		myfile.get(*flavor_text + offset_text, len);
		(*flavor_text)[len + offset_text - 1] = '\0';
		god.gdebug(DBG_READFILE) << "Read full flavor: '" << *flavor_text << "'\n";
		if(myfile.get() == '}') return true;
		else return false;
	}
}

bool read_section_abils(std::ifstream& myfile, int* nb_actabs, PermOption** first_actab){
	if(*nb_actabs != 0) raise_error("Ability text of this spell declared multiple times");
	char a = myfile.get();
	int i = 0, v = 0;
	if(a == '<') v = 1;
	else if(a == '}') v = 2;
	else{
		*nb_actabs = 1;
		while(v == 0){ //counting number of abilities
			char a = myfile.get();
			switch(a){
				case '/': (*nb_actabs)++; break;
				case '<': v = 1; break;
				case '}': v = 2; break;
			}
			i++;
		}
		myfile.seekg(-i-1, std::ios_base::cur); //going back where we were
		*first_actab = new PermOption[*nb_actabs];
		for(int i=0; i<*nb_actabs; i++){
			bool tapsymbol = false;
			Mana cost = 0;
			bool ismanaability = false;
			PreResolvable* effects = 0;
			while(1){
				//reading ability cost
				char copy[30];
				int i = 0;
				do{
					copy[i++] = myfile.get();
					if(i==30){
						raise_error("Error: cost is longer than 30 symbols; are you sure you are declaring the cost of an activated ability?");
					}
				} while(copy[i-1] != ',' && copy[i-1] != ':');//reading one cost
				god.gdebug(DBG_X_READFILE) << "Read" << copy << std::endl;
				if(copy[0] == 'T'){
					tapsymbol = true;
				}
				else{
					cost = t2m(copy);
					god.gdebug(DBG_X_READFILE) << copy << " was translated to " << cost << std::endl;
				}
				if(copy[i-1] == ':') break;
			}
			god.gdebug(DBG_READFILE) << "Tap symbol:" << tapsymbol <<", cost: " << m2t(cost) << std::endl;
			if(myfile.get() == ':') ismanaability = true; //reading either : or ' '
			read_section_onresolve(myfile, &effects); //stops after reading '/'
			//the last stops before reading '<'
			(*first_actab)[i].fillout(cost, effects, tapsymbol, ismanaability);
		}
	}
	if(v == 2) return true;
	else return false;
}

bool read_section_triggers(std::ifstream& myfile, int* nb_triggers, char** typeof_triggers, Trigger** triggers){
	static Dictionary dict_triggertypes(olympus::nb_trigtypes, olympus::all_trigtypes);
	if(*nb_triggers != 0) raise_error("Triggers of this spell declared multiple times");
	char a = myfile.get();
	int v = 0;
	if(a == '<') v = 1;
	else if(a == '}') v = 2;
	else{
		*nb_triggers = 1;
		long int startpos = myfile.tellg();
		while(v == 0){ //counting number of triggers
			switch(myfile.get()){
				case '/': (*nb_triggers)++; break;
				case '<': v = 1; break;
				case '}': v = 2; break;
			}
		}
		myfile.seekg(startpos-1, std::ios_base::beg); //we're going back one more character since we've read a first character before the if statement
		*triggers = new Trigger[*nb_triggers];
		*typeof_triggers = new char[*nb_triggers];
		for(int i=0; i<*nb_triggers; i++){
			char trigtype_tmp[20];
			myfile.get(trigtype_tmp, 20, ':');
			check_safepoint(myfile, ':', "before triggered ability");
			myfile.get(); //getting ' ' or ':'
			int trigtype = dict_triggertypes.find(trigtype_tmp);
			if(trigtype == -1){
				raise_error(std::string("trigger ") + trigtype_tmp + " does not exist");
			}
			(*typeof_triggers)[i] = (char) trigtype;
			PreResolvable* effects = 0;

			read_section_onresolve(myfile, &effects); //stops after reading '/'
			if(i != *nb_triggers-1) check_safepoint(myfile, ' ', "after a /");
			//the last stop after reading '<' or '}'
			(*triggers)[i].setEffects(effects);
		}
	}
	if(v == 2) return true;
	else return false;
}

/*
Most abilities are of the form do_something(target Blob, value x), like deal x damage to target Blob.
target can be 'target Thing (with characteristics)' [target (tapped) creature], 'all Things' (with characteristics), or 'you'.
An ability can also imply two targets 'target creature becomes a copy of another target creature', 'fights target creature', 'exchange life totals'
*/

bool read_section_onresolve(std::ifstream& myfile, PreResolvable** preRes){ //reads up to ending character: '<', '}' or '/'
//returns true if it is the end of rules text '}', and else false
	if(*preRes != 0){
		raise_error("OnResolve abilities of this spell declared multiple times"); return 0;
	}
	else{
		if(myfile.peek() == '<'){ myfile.get(); return false; } //if section ends immediately
		else if(myfile.peek() == '}'){ myfile.get(); return true; }
		else{
			*preRes = new PreResolvable();
			return (*preRes)->init(myfile);
		}
	}
}

bool PreResolvable::init(std::ifstream& myfile){ //reads from start of parameter (not ' ', which will be interpreted as void parameter!)
//up to the end character: '<', '}' or '/'
	bool is_the_end = false;
	char allassignedvariables[256] = {0};
	int nbassignedparams = 0;
	while(1){
		char p1 = read_ability_parameter(myfile, allassignedvariables, &nbassignedparams);
		int type = read_ability_type(myfile); //reads up to the space after the ability
		char p2 = read_ability_parameter(myfile, allassignedvariables, &nbassignedparams); //reads up to the symbol after the parameter
		
		fab = std::make_unique<Ability>(fab, p1, (char) type, p2);
		
		check_safepoint(myfile, '.', "after an ability");
		char a = myfile.get(); //we were at '.' in '{you draw 1.}' ; now we're at the symbol after
		god.gdebug(DBG_READFILE) << "Read " << a << std::endl;
		if(a == '}'){
			is_the_end = true;
			break;
		}
		else if(a == '<' || a == '/') break;
	}

	parameters = new char[nbassignedparams];
	nb_parameters = nbassignedparams;
	
	unsigned char i = 0;
	do{ //putting all parameters from the hashtable 'allassignedvariables' into the array 'parameters'
		//printf("[%d] - %d\n", i, allassignedvariables[i]);
		if(allassignedvariables[(int) i] != 0){
			char value = (i&0xf0) + ((allassignedvariables[(int) i]&0xf0) >> 4);
			char index = allassignedvariables[(int) i]&0x0f;
			parameters[index - 1] = value;
		}
		i++;
	} while(i!=0);
	return is_the_end;
}