#define MANUAL_IMPORT_OF_OLYMPUS_HEADERS
#include ".header_link.h"
#include "../include/7game.h"

int main(){
    std::cout << sizeof(Mana) << std::endl;
	Game maingame("DimirJace", "GruulChandra", DBG_IMPORTANT | DBG_READFILE);
	maingame.play();
	return 0;
}