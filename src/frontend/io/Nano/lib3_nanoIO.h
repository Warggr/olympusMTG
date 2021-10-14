#ifndef OLYMPUS_11_NANOIO
#define OLYMPUS_11_NANOIO

#include "../6abstractIO.h"

#include <iostream>

/*Nano will try to reverse-engineer iteration and in-game mouse movement*/
class NanoIO: public AbstractIO {
	bool is_iterating;
public:
	NanoIO();
	~NanoIO();

#include "../iomethods.cpp"
};

inline void NanoIO::disp(const CardOracle& oracle, const Rect&, bool) const { poster(oracle); }

#endif //OLYMPUS_11_NANOIO