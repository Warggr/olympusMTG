#define MANUAL_IMPORT_OF_OLYMPUS_HEADERS
#include ".header_link.h"
#include "../include/8game.h"

int main(){
	Game maingame("DimirJace", "GruulChandra", DBG_IMPORTANT | DBG_TARGETING);
	maingame.play();
	return 0;
}