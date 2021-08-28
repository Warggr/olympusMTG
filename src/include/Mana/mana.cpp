#include "lib2_mana.h"
#include <ostream>

#define MY_COLOR(mana, i) (((mana) >> 4*i)&0xf)
#define COLOR_MASK(i) (15 << 4*i)

bool Mana::operator>=(Mana cost) const {
	for(int i=0; i<7; i++){ //checking total and all five colors
		if(MY_COLOR(cost.mana, i) > MY_COLOR(mana, i)) return false;
	}
	return true;
}

bool Mana::operator>(Mana cost) const{
	if(MY_COLOR(mana, positions::GENERIC) > MY_COLOR(cost.mana, positions::GENERIC)) return true;
	return false;
}

bool Mana::operator<(Mana cost) const{
	if(MY_COLOR(mana, positions::GENERIC) < MY_COLOR(cost.mana, positions::GENERIC)) return true;
	return false;
}

colorId::type Mana::m2color() const {
	char color = 0;
	for(int i=positions::FIRST_COLOR; i<=positions::LAST_COLOR; i++){
		if(mana & COLOR_MASK(i)){
			color += 1 << (i - 1);
		}
	}
	return color;
}

void Mana::operator-=(Mana cost){
	mana -= (cost.mana & 0xfffffff); //removing total and all six types of mana
	//doing a checksum and removing the necessary colored mana
	uint generic = cmc(); //The correct total mana that should be left
	for(uint offset = positions::FIRST_COLOR; offset <= positions::COLORLESS; offset++){
		uint color = MY_COLOR(mana, offset);
		if(color < generic) generic -= color;
		else{
			mana = mana - ((color - generic) << 4*offset);
			mana = mana & ~(~0U << (4*offset + 4));
			return;
		}
	}
}

Mana::Mana(const char* x){ //non-mana symbols are considered terminator symbols
	//which does not mean there's a such thing as mana used for cards depicting the Terminator, but that the string ends there
	mana = 0;
	int pos = 0;
	while(x[pos] >= '0' && x[pos] <= '9'){
		mana = 10*mana + x[pos++]-'0';
	}
	while(true){
		switch(x[pos++]){
			case 'W': mana += 1 + (1 << (4*1)); break;
			case 'U': mana += 1 + (1 << (4*2)); break;
			case 'B': mana += 1 + (1 << (4*3)); break;
			case 'R': mana += 1 + (1 << (4*4)); break;
			case 'G': mana += 1 + (1 << (4*5)); break;
			case 'C': mana += 1 + (1 << (4*6)); break;
			default: return;
		}
	}
}

std::string Mana::m2t() const {
	std::string ret;
	int mcopy = mana;
	const char* colors = "WUBRGC";
	uint generic = cmc();
	for(int i=0; i<6; i++){
		mcopy = mcopy >> 4;
		for(int j=0; j < (mcopy&0xf); j++) ret += colors[i];
	}
	generic -= ret.length();
	if(generic!=0) ret = std::to_string(generic) + ret;
	return ret;
}

void Mana::operator+=(char color){
	mana += (1 << (4*color)) + 1;
}

void Mana::operator+=(Mana m){
	mana += m.mana;
}

std::ostream& operator<<(std::ostream& os, const Mana& m){
	os << m.m2t();
	return os;
}

ExplicitMana Mana::m2x() const {
    auto ret = ExplicitMana();
    uint generic = cmc();
    int mcopy = mana;
    for(int i=0; i<6; i++){
        mcopy = mcopy >> 4;
        ret[i+1] = mcopy & 0xf;
        generic -= ret[i+1];
    }
    ret[0] = generic;
    return ret;
}
