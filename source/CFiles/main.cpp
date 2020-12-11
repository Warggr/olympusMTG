#include "../HFiles/olympus_main.h"

int main(){
	Game maingame("Materials/decks/magicDecks.txt", DBG_IMPORTANT | DBG_TARGETING);
	maingame.play();
	return 0;
}