#include "lib1_dict.h"
#include "filereader.h"
#include "headE_enums.h"
#include "build_types.h"
#include <cstring>
#include <iostream>

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
			if(txt[3] == 'l') return target_type::resolvable;
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
	if(ret == dicts->dict_abiltypes.not_found){
		throw DeckbuildingError(std::string("Invalid ability type '") + ability + "'");
	}
	else return *ret;
}

short int PlainFileReader::readNumber(char a, bool can_be_zero, bool can_be_negative){
	if(('0' <= a && a <= '9') || a == '-'){
		short int b;
		ifile >> b;
		gdebug(DBG_READFILE) << "Read number literal " << b << "-";
		if(!can_be_zero && b == 0) throw DeckbuildingError("Invalid ability parameter: expected non-0 number");
		if(!can_be_negative && b < 0) throw DeckbuildingError("Invalid ability parameter: expected positive number");
		return b;
	}
	else{ throw DeckbuildingError("Invalid ability parameter: expected a number"); }
}

inline mtg::manatype read_mana_to_add(char a){
	switch(a){
	    case 'C': return mtg::manatype::colorless;
		case 'W': return mtg::manatype::white;
		case 'U': return mtg::manatype::blue;
		case 'B': return mtg::manatype::black;
		case 'R': return mtg::manatype::red;
		case 'G': return mtg::manatype::green;
		default: return mtg::manatype::colorless;
	}
}

flag_t PlainFileReader::readAbilityParameter(char* allassignedvariables, uint8_t& nbassignedparams, flag_t type){ //reads only the parameter itself.
	check_safepoint(' ', "before a parameter");
	char a = ifile.peek();
	gdebug(DBG_READFILE) << "Read parameter starting with " << a << "\n";
	if(type == target_type::added_mana){ ifile.get(); return (flag_t) read_mana_to_add(a); }
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
			if(strcmp("this", you) == 0) return -2; //-2 is 'this'
			else {throw DeckbuildingError("The only valid parameter starting with 't' is 'this'"); }
		}
		case 'e':{
			char you[10];
			ifile.get(you, 10);
			if(strcmp("enchanted", you) == 0) return -3; //-3 is 'enchanted or equipped permanent'
			else {throw DeckbuildingError("The only valid parameter starting with 'e' is 'enchanted'"); }
		}
		case '%':{ //possible options: local, planeswalker, permanent, out_of_hand_card, spell, creature
			char types = 0;
			//(additional constraints, such as 'target Minotaur', or even 'target artifact', will be saved somewhere else, I don't really care)
			ifile.get(); //getting %
			while(true){
				char txt[20];
				int i=0;
				while(ifile.peek() >= 'a' && ifile.peek() <= 'z'){
					txt[i++] = ifile.get();
				}
				types = types | readTypeofTarget(txt);
				if(ifile.peek() != '|') break;
				else ifile.get(); //getting '|'
			}
			int b;
			if(ifile.peek() > '9' || ifile.peek() < '1') { throw DeckbuildingError("you forgot to give the target a number >= 1"); return 0; }
			ifile >> b;
			int index = (types&0xf0) + b;
			if(allassignedvariables[index] == 0){
				nbassignedparams++;
				allassignedvariables[index] = nbassignedparams + 16*types; //rest of the type flags and var number;
				//the rest of the infos are in the index
				return nbassignedparams;
			}
			else{
				return allassignedvariables[index] & 0x0f;
			}
		}
		default:
			throw DeckbuildingError(std::string("Invalid ability parameter starting with ") + a +
			" for type " + std::to_string(static_cast<int>(type)));
	}
}

void PlainFileReader::readCardType(card_type& type) {
    char a = ifile.get();
    if(a == 'L') { type.legendary = 1; a = ifile.get(); }
    if(a == 'W') { type.snow = 1; a = ifile.get(); }
    if(a == 'B') { type.shift = 1; a = ifile.get(); }
    if(a == 'E') { type.enchantment = 1; a = ifile.get(); }
    if(a == 'A') { type.artifact = 1; a = ifile.get(); }
    if(a == 'L') { type.land = 1; a = ifile.get(); }
    switch (a) {
        case ' ': return; //underlying is 'flagged' by default
        case 'I': type.underlying = card_type::sorcery; type.shift = 1; break;
        case 'P': type.underlying = card_type::planeswalker; break;
        case 'C': type.underlying = card_type::creature; break;
        case 'S': type.underlying = card_type::sorcery; break;
        default:
            raise_error(std::string("while reading decks: ") + a + " is not a card type");
    }
    check_safepoint(' ', "after card types");
}

void PlainFileReader::readCosts(Cost& cost, bool& tapsymbol) {
    while(true){
        char copy[30];
        int j;
        for(j=0; j<30; j++) {
            copy[j] = ifile.get();
            if(copy[j] == ',' || copy[j] == ':') break; //reading one cost
        }
        if(j==30) raise_error("Error: cost is longer than 30 symbols; are you sure you are declaring the cost of an activated ability?");
        if(copy[0] == 'T') {
            tapsymbol = true;
        } else {
            cost.mana = Mana(copy);
        }
        if(copy[j] == ':') break;
    }
}
