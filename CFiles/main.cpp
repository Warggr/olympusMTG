#include "../include/.olympus_main.h"
#include "../include/7game.h"
#include <iostream>

int main(){
	Game maingame("DimirJace", "GruulChandra", DBG_IMPORTANT | DBG_READFILE);
	maingame.play();
	return 0;
}