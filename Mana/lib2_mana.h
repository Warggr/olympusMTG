#ifndef OLYMPUS_HEADER_2_MANA_H
#define OLYMPUS_HEADER_2_MANA_H

#include <string>

class Mana{
private:
    int mana; //0xXcgrbuw1, in inverse order of priority
public:
	Mana(): mana(0){};
    Mana(int i): mana(i){};
	Mana(const char* x);
	std::string m2t() const;
	bool operator>=(Mana pool) const;
	bool operator>(Mana pool) const;
	void operator-=(Mana cost);
	void operator+=(char color);
	void operator+=(Mana m);
	char m2color() const;
	void disp(int x, int y) const;
	int m2i() const {return mana; };

	friend std::ostream& operator<<(std::ostream& os, const Mana& m);
};

#endif //OLYMPUS_HEADER_MANA_2_H