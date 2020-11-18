#include <cstdlib>
#include <cstdio>
#include "../HFiles/classes.h"

void read_rules_text(FILE* myfile, Ability** fab, char** flavor_text, char** parameter_list, int* nb_parameters, int* nb_actabs, PermOption** first_actab, char type){
	check_safepoint(myfile, '{', "at the beginning of the rules text");
	int offset_text = (type==4) ? 2 : (type==3) ? 1 : 0; //2 for creatures, 1 for planeswalkers
	if(fgetc(myfile) == '}'){
		*fab = 0;
		*flavor_text = 0;
		*nb_parameters = 0;
		*nb_actabs = 0;
	}
	else{
		char section_name = 1; //1 for 'ability text'
		fseek(myfile, -1, SEEK_CUR);
		if(type == 0 || type == 5) section_name = 0; //0 for 'oncast/on_etb text'
		while(1){ //loop to read all sections
			god.gdebug(DBG_READFILE) << "Expecting section n°" << section_name << std::endl;
			bool is_the_end;
			switch(section_name){
				case 0: is_the_end = read_section_onresolve(myfile, fab, nb_parameters, parameter_list); break;
				case 1: is_the_end = read_section_abils(myfile, nb_actabs, first_actab); break;
				case 2: is_the_end = read_section_flavor(myfile, flavor_text, offset_text); break;
				//each stops after reading '<'
			}
			if(is_the_end) break;
			else{
				switch(fgetc(myfile)){
					case 'o': section_name = 0; break;
					case 'a': section_name = 1; break;
					case 'x': section_name = 2; break;
				}
				check_safepoint(myfile, '>', "after declaring another section");
			}
		}
	}
	if(*fab == 0){
		*parameter_list = 0;
		*nb_parameters = 0;
	}
	if(offset_text != 0){
		if(!(*flavor_text)) (*flavor_text) = new char [offset_text];
		check_safepoint(myfile, ' ', "before p/t or loyalty");
		if(type == 4){ //getting power and toughness
			int power, toughness;
			fscanf(myfile, "%d/%d", &power, &toughness);
			(*flavor_text)[0] = (char) power;
			(*flavor_text)[1] = (char) toughness;
		}
		if(type == 3){ //getting loyalty
			int loyalty;
			check_safepoint(myfile, '\\', "just before loyalty number");
			fscanf(myfile, "%d", &loyalty);
			(*flavor_text)[0] = (char) loyalty;
			check_safepoint(myfile, '/', "just after loyalty number");
		}
	}
	check_safepoint(myfile, '\n', "after card declaration");
}

bool read_section_flavor(FILE* myfile, char** flavor_text, char offset_text){
	if(*flavor_text != 0){
		god.gdebug(DBG_X_READFILE | DBG_IMPORTANT) << "Error: Flavor text of this spell declared multiple times\n";
		god.minimalKill();
		printf("Error: Flavor text of this spell declared multiple times\n");
		exit(1);
	}
	int len = 0;
	char a;
	while((a = fgetc(myfile))){
		len++;
		if(a=='<' || a == '}') break;
	}
	if(len == 1){
		*flavor_text = 0;
		return false;
	}
	else{
		fseek(myfile, -len, SEEK_CUR);
		*flavor_text = new char [len + offset_text];
		for(int j=offset_text; j < len -1 + offset_text; j++){
			(*flavor_text)[j] = fgetc(myfile);
		}
		(*flavor_text)[len - 1 + offset_text] = '\0';
		if(fgetc(myfile) == '}') return true;
		else return false;
	}
}

bool read_section_abils(FILE* myfile, int* nb_actabs, PermOption** first_actab){
	if(*nb_actabs != 0){
		god.gdebug(DBG_X_READFILE | DBG_IMPORTANT) << "Error: Ability text of this spell declared multiple times\n";
		god.minimalKill();
		printf("Error: Ability text of this spell declared multiple times\n");
		exit(66);
	}
	char a = fgetc(myfile);
	int i = 0, v = 0;
	if(a == '<') v = 1;
	else if(a == '}') v = 2;
	else{
		*nb_actabs = 1;
		while(v == 0){ //counting number of abilities
			char a = fgetc(myfile);
			switch(a){
				case '/': (*nb_actabs)++; break;
				case '<': v = 1; break;
				case '}': v = 2; break;
			}
			i++;
		}
		fseek(myfile, -i-1, SEEK_CUR); //going back where we were
		*first_actab = new PermOption[*nb_actabs];
		for(int i=0; i<*nb_actabs; i++){
			bool tapsymbol = false;
			Mana cost = 0;
			bool ismanaability = false;
			int nbtargets = 0;
			char* typesoftargets = 0;
			Ability* fab = 0;
			while(1){
				//reading ability cost
				char copy[30];
				int i = 0;
				do{
					copy[i++] = fgetc(myfile);
					if(i==30){
						god.gdebug(DBG_X_READFILE | DBG_IMPORTANT) << "Error: cost is longer than 30 symbols; are you sure you are declaring the cost of an activated ability?\n";
						god.minimalKill();
						printf("Error: cost is longer than 30 symbols; are you sure you are declaring the cost of an activated ability?\n");
						exit(1);
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
			if(fgetc(myfile) == ':') ismanaability = true; //reading either : or ' '
			read_section_onresolve(myfile, &fab, &nbtargets, &typesoftargets); //stops after reading '/'
			//the last stops before reading '<'
			(*first_actab)[i].fillout(cost, nbtargets, typesoftargets, fab, tapsymbol, ismanaability);
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
bool read_section_onresolve(FILE* myfile, Ability** ab, int* nbargs, char** args){
	bool is_the_end = false;
	if(*ab != 0){
		god.minimalKill();
		printf("Error: OnResolve abilities of this spell declared multiple times\n");
		exit(3);
	}
	char allassignedvariables[256] = {0};
	int nbassignedparams = 0;
	while(1){
		char a = fgetc(myfile);
		god.gdebug(DBG_READFILE) << "Read separator: " << a << std::endl;
		if(a == '<') break; //this will happen only if the section ends immediately
		else fseek(myfile, -1, SEEK_CUR);

		char p1 = read_ability_parameter(myfile, allassignedvariables, &nbassignedparams, false);
		int type = read_ability_type(myfile); //reads up to the space after the ability
		char p2 = read_ability_parameter(myfile, allassignedvariables, &nbassignedparams, true); //reads up to the symbol after the parameter
		*ab = new Ability(*ab, p1, (char) type, p2);
		
		a = fgetc(myfile); //we were at '.' in '{you draw 1.}' ; now we're at the symbol after
		//if(god) god.debug_log, "After-param: %d ('%c')\n", a, a);
		if(a == '}' || a == '/'){
			is_the_end = true;
			break;
		}
		else if(a == '<') break;
	}
	*args = new char[nbassignedparams];
	*nbargs = nbassignedparams;
	unsigned char i = 0;
	do{
		//printf("[%d] - %d\n", i, allassignedvariables[i]);
		if(allassignedvariables[(int) i] != 0){
			char value = (i&0xf0) + ((allassignedvariables[(int) i]&0xf0) >> 4);
			char index = allassignedvariables[(int) i]&0x0f;
			(*args)[index - 1] = value;
		}
		i++;
	} while(i!=0);
	return is_the_end;
}

int read_ability_type(FILE* myfile){
	const char* name_abilities[NB_DIFFERENT_ABILITIES] = ALL_DIFFERENT_ABILITIES;
	int i;
	for(i=0; i<=NB_DIFFERENT_ABILITIES; i++){
		if(i== NB_DIFFERENT_ABILITIES){
			char tmp[100]; int j=0;
			while((tmp[j++] = fgetc(myfile)) != ' ');
			god.gdebug(DBG_X_READFILE | DBG_IMPORTANT) << "Error while parsing decks: Ability '" << tmp << "' does not exist\n";
			god.minimalKill();
			exit(22);
		}
		int j=0, v=1;
		while(v){
			if(name_abilities[i][j] == '\0' && fgetc(myfile) == ' '){ v=0; break; }
			else if(name_abilities[i][j] != fgetc(myfile)) break;
			j++;
		}
		if(!v) break;
		fseek(myfile, -j-1, SEEK_CUR);
	}
	return i;
}

char read_ability_parameter(FILE* myfile, char* allassignedvariables, int* nbassignedparams, bool secondtime){ //reads up to the symbol after the parameter; or reads only the space of void parameter
	char a = fgetc(myfile);
	god.gdebug(DBG_READFILE) << "Read parameter starting with" << a << std::endl;
	if(a == 'y'){
		if(fgetc(myfile) == 'o' && fgetc(myfile) == 'u'){
			fgetc(myfile); //' ' or '.' or ';'
			return 0;
		}
		else {
			god.minimalKill();
			printf("Error: The only valid parameter starting with 'y' is 'you '\n");
			exit(32);
		}
	}
	if(a == ' ' || a == '.'){
		return 0;
	}
	if(a == '%'){ //possible options: player, planeswalker, permanent, card, spell, creature
		char types = 0;
		//(additional constraints, such as 'target Minotaur', or even 'target artifact', will be saved somewhere else, I don't really care)
		do{
			types = types | read_typeof_target(myfile);
			while((a= fgetc(myfile))) if(!('a' <= a && a <= 'z')) break; //getting the remaining letters
		} while(a == '|');
		a = a - '0';
		while(char b = fgetc(myfile)){
			if(b == ' ') break;
			a = a*10 + b - '0';
		}
		int index = (types&0xf0) + a;
		if(allassignedvariables[index] == 0){
			(*nbassignedparams)++;
			allassignedvariables[index] = *nbassignedparams + 16*types; //rest of the type flags and var number; the rest of the infos are in the index
			return (char) *nbassignedparams;
		}
		else{
			return allassignedvariables[index] & 0x0f;
		}
	}
	bool positive = true;
	if(a == '-'){
		a = fgetc(myfile);
		positive = false;
	}
	if('0'<=a && a<='9'){ //reading a number
		a -= '0';
		char b;
		char end = ' '; if(secondtime) end = '.';
		while((b = fgetc(myfile)) != end){
			a = 10*a + b - '0';
		}
		if(!positive) a = -a;
		return a;
	}
	else{
		god.gdebug(DBG_X_READFILE | DBG_IMPORTANT) << "Error: invalid ability parameter.\n";
		god.minimalKill();
		exit(1);
	}
}

char read_typeof_target(FILE* myfile){ //player 80 - permanent 40 - spell (well, resolvable) 20 - creature 10
//planeswalker 08 - ? - card 02 - ?
	if(fgetc(myfile) == 'p'){
		if(fgetc(myfile) == 'l'){
			fgetc(myfile); //a
			if(fgetc(myfile) == 'y') return 0x80; //player
			else return 0x08; //PW
		}
		else if(fgetc(myfile) == 'r') return 0x40; //confirmation of 'permanent'; the e was read earlier
	}
	else{
		fgetc(myfile);
		if(fgetc(myfile) == 'e'){
			if(fgetc(myfile) == 'l') return 0x20; //spell
			else return 0x10; //creature
		}
		else return 0x02; //card
	}
	return 0x00;
}