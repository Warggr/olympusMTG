#include "../include/.olympus_main.h"
#include "../include/7game.h"
#include <iostream>

int main(){
    std::cout << sizeof(Mana) << std::endl;
	Game maingame("DimirJace", "GruulChandra", DBG_IMPORTANT | DBG_READFILE);
	maingame.play();
	return 0;
}