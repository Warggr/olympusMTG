#include "Dictionary/lib1_dictionary.h"
#include "headR_readfiles.h"
#include "../0/headC_constants.h"
#include <cstring>
#include <iostream>

template<>
Dictionary_tpl<int>::iterator Dictionary_tpl<int>::not_found(0);

flag_t readTypeofTarget(const char* txt){
	if(txt[0] == 'p'){
		if(txt[1] == 'l'){
			if(txt[3] == 'y') return target_type::player;
			else return target_type::planeswalker;
		}
		else if(txt[2] == 'r') return target_type::permanent; //the e was read earlier
	}
	else{
		if(txt[2] == 'e'){
			if(txt[3] == 'l') return target_type::spell;
			else return target_type::creature;
		}
		else return target_type::card;
	}
	return target_type::none;
}

effect_type PlainFileReader::readAbilityType(){ //gets only the action
	char ability[30];
	ifile >> ability;
	auto ret = dicts->dict_abiltypes.find(ability);
	if(ret == Dictionary::not_found){
		throw DeckbuildingError(std::string("Invalid ability type '") + ability + "'");
	}
	else return static_cast<effect_type>(*ret);
}

short int PlainFileReader::readNumber(char a, bool can_be_zero, bool can_be_negative){
	if(('0' <= a && a <= '9') || a == '-'){
		short int b;
		ifile >> b;
		gdebug(DBG_READFILE, "Read number literal " << b << "-");
		if(!can_be_zero && b == 0) throw DeckbuildingError("Invalid ability parameter: expected non-0 number");
		if(!can_be_negative && b < 0) throw DeckbuildingError("Invalid ability parameter: expected positive number");
		return b;
	}
	else{ throw DeckbuildingError("Invalid ability parameter: expected a number"); return 0; }
}

punctuation PlainFileReader::getPunctuation() {
	switch(ifile.get()){ 
		case ',': return CONTINUE;
		case '.': return ENDPOINT;
		case ';': return BREAKOUT;
		default: throw DeckbuildingError("Punctuation missing");
	}
}

enum mtgmanatype { white, blue, black, red, green, colorless };

inline mtgmanatype read_mana_to_add(char a){
	switch(a){
		case 'C': return colorless;
		case 'W': return white;
		case 'U': return blue;
		case 'B': return black;
		case 'R': return red;
		case 'G': return green;
		default: return colorless;
	}
}

int8_t PlainFileReader::readAbilityParameter(char* allassignedvariables, uint8_t* nbassignedparams, flag_t type){ //reads only the parameter itself.
	check_safepoint(' ', "before a parameter");
	char a = ifile.peek();
	gdebug(DBG_READFILE, "Read parameter starting with " << a << "\n");
	if(type == target_type::added_mana){ ifile.get(); return read_mana_to_add(a); }
	if(type & target_type::number)
		return readNumber(a, (type & target_type::nonzero) != target_type::nonzero, (type & target_type::nonnegative) != target_type::nonnegative);
	switch(a){
		case 'y':{
			char you[4];
			ifile.get(you, 4);
			if(strcmp("you", you) == 0) return -1; //-1 is 'you'
			else { throw DeckbuildingError("The only valid parameter starting with 'y' is 'you'"); }
		}
		case 't':{
			char you[5];
			ifile.get(you, 5);
			if(strcmp("this", you) == 0) return -2; //-1 is 'this'
			else {throw DeckbuildingError("The only valid parameter starting with 't' is 'this'"); }
		}
		case 'e':{
			char you[10];
			ifile.get(you, 10);
			if(strcmp("enchanted", you) == 0) return -3; //-1 is 'enchanted or equipped permanent'
			else {throw DeckbuildingError("The only valid parameter starting with 'e' is 'enchanted'"); }
		}
		case '%':{ //possible options: player, planeswalker, permanent, out_of_hand_card, spell, creature
			char types = 0;
			//(additional constraints, such as 'target Minotaur', or even 'target artifact', will be saved somewhere else, I don't really care)
			ifile.get(); //getting %
			while(1){
				char txt[20];
				int i=0;
				while(ifile.peek() >= 'a' && ifile.peek() <= 'z'){
					txt[i++] = ifile.get();
				}
				types = types | read_typeof_target(txt);
				if(ifile.peek() != '|') break;
				else ifile.get(); //getting '|'
			}
			int b;
			if(ifile.peek() > '9' || ifile.peek() < '1') { throw DeckbuildingError("you forgot to give the target a number >= 1"); return 0; }
			ifile >> b;
			int index = (types&0xf0) + b;
			if(allassignedvariables[index] == 0){
				(*nbassignedparams)++;
				allassignedvariables[index] = *nbassignedparams + 16*types; //rest of the type flags and var number;
				//the rest of the infos are in the index
				return *nbassignedparams;
			}
			else{
				return allassignedvariables[index] & 0x0f;
			}
		}
		default:
			throw DeckbuildingError(std::string("Invalid ability parameter starting with ") + a + " for type " + std::to_string(static_cast<int>(type)
    }
}
