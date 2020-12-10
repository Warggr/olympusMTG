#include "../HFiles/olympus_main.h"

typedef int Mana; //0xXcgrbuw1, in inverse order of priority

bool less(Mana cost, Mana pool){
	for(int i=0; i<7; i++){ //checking total and all five colors
		if(((cost >> 4*i)&0xf) > ((pool >> 4*i)&0xf)) return false;
	}
	return true;
}

char mana2color(Mana m){
	char color = 0;
	for(int i=0; i<6; i++){
		if(m & (15 << 4*i)){
			if(color == 0) color = i;
			else color = 6;
		}
	}
	return color;
}

Mana minus(Mana pool, Mana cost){
	pool -= (cost & 0xfffffff); //removing total and all six types of mana
	//doing a checksum and removing the necessary colored mana
	char generic = pool&0xf;
	int offset = 0;
	while(1){
		offset++;
		if(offset == 7){
			return pool;
		}
		char color = (pool >> 4*offset)&0xf;
		if(color < generic) generic -= color;
		else{
			pool = pool - ((color - generic) << 4*offset);
			pool = pool & ~(~0 << (4*offset + 4));
			return pool;
		}
	}
}

Mana t2m(const char* x){ //non-mana symbols are considered terminator symbols
	//which does not mean there's a such thing as mana used for cards depicting the Terminator, but that the string ends there
	Mana ret = 0;
	int pos = 0;
	while(x[pos] >= '0' && x[pos] <= '9'){
		ret = 10*ret + x[pos++]-'0';
	}
	while(1){
		switch(x[pos++]){
			case 'W': ret += 1 + (1 << (4*1)); break;
			case 'U': ret += 1 + (1 << (4*2)); break;
			case 'B': ret += 1 + (1 << (4*3)); break;
			case 'R': ret += 1 + (1 << (4*4)); break;
			case 'G': ret += 1 + (1 << (4*5)); break;
			case 'C': ret += 1 + (1 << (4*6)); break;
			default: return ret;
		}
	}
}

std::string m2t(Mana x){
	std::string ret;
	const char* colors = "WUBRGC";
	char generic = (char) (x&0xf);
	for(int i=0; i<6; i++){
		x = x >> 4;
		for(int j=0; j < (x&0xf); j++) ret += colors[i];
	}
	generic -= ret.length();
	if(generic!=0) ret = std::to_string(generic) + ret;
	return ret;
}

void Player::add_mana(char color){
	manapool += (1 << (4*color)) + 1;
}