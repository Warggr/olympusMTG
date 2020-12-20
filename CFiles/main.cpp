#include "../HFiles/olympus_main.h"

int main(){
	Game maingame("DimirJace", "GruulChandra", DBG_IMPORTANT | DBG_TARGETING);
	maingame.play();
	return 0;
}