#include "../HFiles/classes.h"

int main(){
	Game maingame("Materials/magicDecks.txt", DBG_IMPORTANT | DBG_TARGETING);
	maingame.play();
	return 0;
}