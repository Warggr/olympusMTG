#include "12defaultUI.h"
#include "lib3_IO.h"
#include <iostream>

//AbstractUI* newUILib(AbstractIO* IOLib){ return new DefaultUI(IOLib); }

#define LOGLEN 7

DefaultUI::DefaultUI(AbstractIO* IOLib): myIO(IOLib), screen(3, UIElement::horizontal) {
    int screenW, screenH;
    IOLib->getResolution(screenW, screenH, mouseSupport, linesize);
    screen.init(0, 0, screenW, screenH);
    gdebug(DBG_IOUI) << "Received resolution" << screenW << "x" << screenH << ", linesize " << linesize << "\n";

    auto *board = new ModernElement(2, UIElement::vertical, 0, 0, screenW - screenW / 4 - screenW / 5, screenH);
    auto* leftbar = new ModernElement(2, UIElement::vertical, 0, board->coords.width, screenW / 4, screenH);
    auto* rightbar = new ModernElement(3, UIElement::vertical, 0, leftbar->coords.right(), screenW / 5, screenH);
    screen.addElement(board, 0);
    screen.addElement(leftbar, 1);
    screen.addLastElement(rightbar);

    optionZone = new ListElement(0, 0, rightbarW, optionZSize, optionZSize+5, 10, UIElement::vertical);
    int posterH = screenH * 0.6;
    optionZSize = 2*linesize;
    auto* posterZone = new Rectangle(0, 0, 0, posterH);
    rightbar->addElement(posterZone, 0);
    auto* stackZone = new ListElement(posterH, boardW, 0, 0, optionZSize, 0, UIElement::vertical);
    rightbar->addLastElement(stackZone);

    playerH = screenH / 15; if(playerH < 2*linesize) playerH = 2*linesize;

    permanentZSize = (screenH - 2*playerH) / 12;
    if(permanentZSize < 3*linesize) permanentZSize = (screenH - 2*playerH) / 10;
    permanentYSize = boardW / 10;

    int permanentZOverlap = permanentZSize/2;
    permanentZMargin = (screenH - 2*playerH - 10*permanentZSize + permanentZOverlap) / 8;

    optionsH = 10*optionZSize;

    iozH = 3*linesize;

    IOLib->harmonize(Rect(boardW, 0, leftbarW, posterH), Rect(boardW+leftbarW, optionsH, rightbarW, iozH), 16); //poster - message - nb windows

    ListElement* playerPerms[2];
    for(int i=0; i<2; i++) {
        playerPerms[i] = new ListElement(0, 0, boardW, screenH / 2, 5* permanentZSize, 4, UIElement::vertical);
        board->addElement(playerPerms[i], i);
    }
    playerY[0] = 0; playerY[1] = 0;
    playerZ[0] = 0; playerZ[1] = playerH + 10*permanentZSize + 8*permanentZMargin - permanentZOverlap;
    //for(int i=0; i<2; i++) IOLib->declare_window(playerY[i], playerZ[i], boardW, playerH);

    stackZone = new ListElement(boardW, posterH + optionZSize, rightbarW, optionZSize, optionZSize+2, 10, UIElement::vertical); //there must be an upper-margin over the stack, for PreRes
    leftbar->addElement(stackZone, 0);
    logbookZone = new ListElement(boardW + leftbarW, stackH+iozH, rightbarW, optionZSize, optionZSize+2, LOGLEN, UIElement::vertical);
    for(int i=0; i<2; i++){
        for(int j=0; j<5; j++){
            int y, z;
            playerPerms[i]->get_coordinates(j, &y, &z);
            permanentZones[i*5 + j] = new ListElement(y, z, permanentYSize, permanentZSize, permanentYSize+5, boardW/(permanentYSize+5), UIElement::horizontal);
        }
    }
}

void DefaultUI::clear_opts(){
	optionZone->erase_background(myIO);
}

/*Window* DefaultUI::new_final_element(int y, int z, int width, int height, int offset, int nbItems, bool direction){
    int yOffset = 0, zOffset = 0;
    if(direction == UIElement::vertical) zOffset = offset; else yOffset = offset;
    myIO->declare_window(y, z, nbItems*yOffset + width, nbItems*zOffset + height);
    return new UIElement(y, z, width, height, offset, nbItems, direction);
    return nullptr;
}*/

/*UIElement* DefaultUI::get_optionzone(){
    return optionZone;
}*/

/*UIElement* DefaultUI::declare_element(const int typeofelement, const char player_id){
    switch(typeofelement){
        case AbstractUI::ELTYPE_STACK:
            return stackZone;
        case AbstractUI::ELTYPE_LOGBOOK:
            return logbookZone;
        default:
            return permanentZones[player_id * 5 - 5 + typeofelement - AbstractUI::ELTYPE_PERMANENTS];
    }
}*/

/* local ()	poster		option ()
   permanents 	stack ()	message
   local ()				logbook ()*/

void DefaultUI::get_player_coords(char player_id, Rect* zone, int* liby, int* libz, int* gravey, int* gravez, int* exily, int* exilz, int* cardzoneW, int* cardzoneH){
	int i = (int) player_id;
	*gravey = linesize + playerY[i];
	*liby = *gravey + boardW/5;
	*exily = *liby + boardW/5;
	zone->y = playerY[i];
	zone->z = playerZ[i];
	zone->width = boardW;
	zone->height = playerH;
	*gravez = linesize + playerZ[i]; *libz = *gravez; *exilz = *gravez;
	*cardzoneW = boardW/5 - 2*linesize; if(*cardzoneW < 2*linesize) *cardzoneW = boardW/5;
	*cardzoneH = playerH;
}
