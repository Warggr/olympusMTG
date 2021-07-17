#include "../Dictionary/lib1_dictionary.h"
#include "../HFiles/headR_readfiles.h"
#include "../HFiles/headC_constants.h"
#include <fstream>
#include <cstring>
#include <iostream>

char read_typeof_target(const char* txt){ //player 80 - permanent 40 - spell (well, resolvable) 20 - creature 10
//planeswalker 08 - ? - card 02 - ?
	if(txt[0] == 'p'){
		if(txt[1] == 'l'){
			if(txt[3] == 'y') return 0x80; //player
			else return 0x08; //PW
		}
		else if(txt[2] == 'r') return 0x40; //confirmation of 'permanent'; the e was read earlier
	}
	else{
		if(txt[2] == 'e'){
			if(txt[3] == 'l') return 0x20; //spell
			else return 0x10; //creature
		}
		else return 0x02; //card
	}
	return 0x00;
}

int read_ability_type(std::ifstream& myfile){ //gets action and the following space
	char ability[100];
	myfile >> ability;
	myfile.get();
	int ret = olympus::dict_abiltypes.find(ability);
	if(ret == -1){
		raise_error(std::string("Invalid ability type '") + ability + "'"); return 0;
	}
	else return ret;
}

char read_ability_parameter(std::ifstream& myfile, char* allassignedvariables, int* nbassignedparams){ //reads only the parameter itself.
	char a = myfile.peek();
	god.gdebug(DBG_READFILE) << "Read parameter starting with " << a << std::endl;
	switch(a){
	case ' ':
		myfile.get();
	case '.': case ',':
		return 0;
	case 'y':{
		char you[4];
		myfile.get(you, 4);
		if(strcmp("you", you) == 0){
			return 0;
		}
		else {
			raise_error("The only valid parameter starting with 'y' is 'you'"); return 0;
		}
	}
	case '%':{ //possible options: player, planeswalker, permanent, card, spell, creature
		char types = 0;
		//(additional constraints, such as 'target Minotaur', or even 'target artifact', will be saved somewhere else, I don't really care)
		myfile.get(); //getting %
		while(1){
			char txt[20];
			int i=0;
			while(myfile.peek() >= 'a' && myfile.peek() <= 'z'){
				txt[i++] = myfile.get();
			}
			types = types | read_typeof_target(txt);
			if(myfile.peek() != '|') break;
			else myfile.get(); //getting '|'
		}
		int b;
		if(myfile.peek() > '9' || myfile.peek() < '1') raise_error("you forgot to give the target a number >= 1");
		myfile >> b;
		int index = (types&0xf0) + b;
		if(allassignedvariables[index] == 0){
			(*nbassignedparams)++;
			allassignedvariables[index] = *nbassignedparams + 16*types; //rest of the type flags and var number;
			//the rest of the infos are in the index
			return (char) *nbassignedparams;
		}
		else{
			return allassignedvariables[index] & 0x0f;
		}
	}
	default:
		if(('0' <= a && a <= '9') || a == '-'){
			int b;
			myfile >> b;
			god.gdebug(DBG_READFILE) << "Read number literal " << b << std::endl;
			return b;
		}
		else{
			raise_error("Invalid ability parameter."); return 0;
		}
	}
}