#include "12defaultUI.h"
#include "lib3_IO.h"
#include <iostream>

#define LOGLEN 7

DefaultUI::DefaultUI(AbstractIO* IOLib): myIO(IOLib) {
    int screenW, screenH;
    IOLib->getResolution(screenW, screenH, mouseSupport, Screen::linesize);
    screen.initScreen(screenW, screenH);
    gdebug(DBG_IOUI) << "Received resolution" << screenW << "x" << screenH << ", linesize " << linesize << "\n";

    IOLib->harmonize(screen.leftBar().poster().coords, screen.rightBar().messageZone().coords, 16); //nb windows
}

void DefaultUI::clear_opts(){
	screen.rightBar().optionZone().erase_background(myIO);
}

/* local ()	poster		option ()
   permanents 	stack ()	message
   local ()				logbook ()*/

void DefaultUI::registerPlayers(std::list<Player>& players) {
    int i=0;
    for (auto iter = players.begin(); i < 2; i++, iter++) {
        screen.board().players()[i].header().setPlayer(&(*iter));
    }
}

void DefaultUI::addCards(const std::list<std::unique_ptr<Card>> &cards) {
    for(auto& card: cards){
        screen.rightBar().optionZone().insert(card.get());
    }
}

void DefaultUI::fullDisp() {
    screen.fullDisp(myIO);
}
