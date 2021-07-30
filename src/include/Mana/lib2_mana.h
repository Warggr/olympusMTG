#ifndef OLYMPUS_HEADER_2_MANA_H
#define OLYMPUS_HEADER_2_MANA_H

#include <string>
#include <array>

using ExplicitMana = std::array<uint, 7>;

class Mana{
private:
    int mana; //0xXcgrbuw1, in inverse order of priority
public:
	Mana(): mana(0){};
    Mana(int i): mana(i){};
	explicit Mana(const char* x);
	std::string m2t() const;
	bool operator>=(Mana pool) const;
	bool operator>(Mana pool) const;
	bool operator<(Mana pool) const;
	void operator-=(Mana cost);
	void operator+=(char color);
	void operator+=(Mana m);
	char m2color() const;
	void disp(int x, int y) const;
	inline int m2i() const {return mana; };
	inline uint cmc() const { return mana & 0xf; }
	ExplicitMana m2x() const;

	friend std::ostream& operator<<(std::ostream& os, const Mana& m);
};

#endif //OLYMPUS_HEADER_2_MANA_H