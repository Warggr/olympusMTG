#include "12defaultUI.h"
#include "../Yggdrasil/headB_board.h"
#include "../include/4permanents.h"

Abstract_ui* new_UILib(Abstract_io* IOLib){return new Default_ui(IOLib); }

void Default_ui::normalize_gridy_gridz(){
	if(myIO->gmouseY() < boardW){
		gridy = 0;
		if(myIO->gmouseZ() < playerH) gridz = 0;
		else if(myIO->gmouseZ() > playerH + 10*permanentZSize - 15) gridz = 11;
		else gridz = (myIO->gmouseZ() - playerH) / (permanentZSize+permanentZMargin) - 1;
	}
	else if(myIO->gmouseY() < boardW + leftbarW){
		gridy = 1;
	}
	else{
		gridy = 2;
	}
}

_UIElement::_UIElement(int left, int top, int _ySize, int _zSize, int offset, int _maxItems, bool direction):
topZ(top), leftY(left), yOffset(0), zOffset(0), ySize(_ySize), zSize(_zSize), maxItems(_maxItems) {
	if(direction == vertical) zOffset = offset;
	else yOffset = offset;
	if(offset < 0){ //reversed zone
		topZ += zOffset;
		leftY += yOffset;
	}
	god.gdebug(DBG_IOUI) << "Dimensions: -" << left <<"-"<< left + maxItems*yOffset + ySize <<"-,|"<< top <<"|"<< top + maxItems*zOffset + zSize << std::endl;
}

UIElement* Default_ui::get_optionzone(){
	return optionZone;
}

_UIElement* Default_ui::new_final_element(int y, int z, int width, int height, int offset, int maxItems, bool direction){
	int yOffset = 0, zOffset = 0;
	if(direction == _UIElement::vertical) zOffset = offset; else yOffset = offset;
	myIO->declare_window(y, z, maxItems*yOffset + width, maxItems*zOffset + height);
	return new _UIElement(y, z, width, height, offset, maxItems, direction);
}

UIElement* Default_ui::declare_element(const int typeofelement, const char player_id){
	switch(typeofelement){
		case Abstract_ui::ELTYPE_STACK:
			return stackZone;
		case Abstract_ui::ELTYPE_LOGBOOK:
			return logbookZone;
		default:
			return permanentZones[player_id * 5 - 5 + typeofelement - Abstract_ui::ELTYPE_PERMANENTS];		
	}
}

Rect _UIElement::get_coordinates(int* yOff, int* zOff) const {
	*yOff = yOffset;
	*zOff = zOffset;
	return Rect(leftY, topZ, ySize, zSize);
}

void _UIElement::get_coordinates(int* y, int* z, int* yOff, int* zOff) const {
	*y = leftY;
	*z = topZ;
	*yOff = yOffset;
	*zOff = zOffset;
}

void _UIElement::get_coordinates(int itemNb, int* y, int* z) const {
	*y = leftY + itemNb * yOffset;
	*z = topZ + itemNb * zOffset;
}

void _UIElement::erase_background(Abstract_io* io) const {
	io->erase_surface(Rect(leftY, topZ, (maxItems-1)*yOffset + ySize, (maxItems-1)*zOffset + zSize));
}

void Default_ui::clear_opts(){
	optionZone->erase_background(myIO);
}

/* player ()	poster		option ()
   permanents 	stack ()	message
   player ()				logbook ()*/
Default_ui::Default_ui(Abstract_io* IOLib): Abstract_ui(IOLib){
	int screenW, screenH;
	IOLib->getResolution(screenW, screenH, mouseSupport, linesize);
	god.gdebug(DBG_IOUI) << "Received resolution" << screenW << "x" << screenH << ", linesize " << linesize << "\n";
	leftbarW = screenW * 0.25; rightbarW = screenW * 0.2; boardW = screenW - leftbarW - rightbarW;
	
	playerH = screenH / 15; if(playerH < 2*linesize) playerH = 2*linesize;
	
	permanentZSize = (screenH - 2*playerH) / 12;
	if(permanentZSize < 3*linesize) permanentZSize = (screenH - 2*playerH) / 10;
	permanentYSize = boardW / 10;

	int permanentZOverlap = permanentZSize/2;
	permanentZMargin = (screenH - 2*playerH - 10*permanentZSize + permanentZOverlap) / 8;
	
	optionZSize = 2*linesize;
	optionsH = 10*optionZSize;

	posterH = screenH * 0.6;
	stackH = screenH - posterH;
	iozH = 3*linesize;

	IOLib->harmonize(Rect(boardW, 0, leftbarW, posterH), Rect(boardW+leftbarW, optionsH, rightbarW, iozH), 16); //poster - message - nb windows
	
	playerY[0] = 0; playerY[1] = 0; 
	playerZ[0] = 0; playerZ[1] = playerH + 10*permanentZSize + 8*permanentZMargin - permanentZOverlap;
	playerPerms[0] = new _UIElement(playerY[0], playerZ[0]+playerH, boardW, permanentZSize, permanentZSize+permanentZMargin, 5, _UIElement::vertical);
	playerPerms[1] = new _UIElement(playerY[1], playerZ[1], boardW, permanentZSize, -permanentZSize-permanentZMargin, 5, _UIElement::vertical);
	for(int i=0; i<2; i++) IOLib->declare_window(playerY[i], playerZ[i], boardW, playerH);
	
	optionZone = new_final_element(boardW + leftbarW, 0, rightbarW, optionZSize, optionZSize+5, 10, _UIElement::vertical);
	stackZone = new_final_element(boardW, posterH + optionZSize, rightbarW, optionZSize, optionZSize+2, 10, _UIElement::vertical); //there must be an upper-margin over the stack, for PreRes
	logbookZone = new_final_element(boardW + leftbarW, stackH+iozH, rightbarW, optionZSize, optionZSize+2, LOGLEN, _UIElement::vertical);
	for(int i=0; i<2; i++){
		for(int j=0; j<5; j++){
			int y, z;
			playerPerms[i]->get_coordinates(j, &y, &z);
			permanentZones[i*5 + j] = new_final_element(y, z, permanentYSize, permanentZSize, permanentYSize+5, boardW/(permanentYSize+5), _UIElement::horizontal);
		}
	}

}

void Default_ui::get_player_coords(char player_id, Rect* zone, int* liby, int* libz, int* gravey, int* gravez, int* exily, int* exilz, int* cardzoneW, int* cardzoneH){
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

bool Default_ui::chooseattackers(PContainer<Creature>& cowards, PContainer<Creature>& warriors, char player_id){
	myIO->message("Choose Attackers");
	bool ret = false;
	int i = 0;
	int yOff, zOff;
	Rect pos = permanentZones[(player_id-1) * 5 + 3]->get_coordinates(&yOff, &zOff);
	for(auto iter = cowards.begin(); iter != cowards.end(); i++){
		if((iter->get_flags()&3) == 3){ //untapped and no summoning sickness
			int zPos = (player_id == 1) ? pos.z + pos.height : pos.z; //getting either top or bottom Z
			bool attacks = myIO->attack_switch(pos.y, pos.width, zPos, (player_id == 1) ? 20 : -20);
			if(attacks){
				auto newatt = iter;
				newatt->disp(pos, false); //disp creature normally
				newatt->reset_flags(1); //tap creature
				iter++;
				newatt.get_pointed()->unstate(); //move creature to list "warriors"
				ret = true;
			}
			else{
				iter->disp(pos, false); //disp creature normally
				iter++;
			}
		}
		else iter++;
		pos.shift(yOff, zOff);
	}
	return ret;
}

void Default_ui::chooseblockers(PContainer<Creature>& defenders, PContainer<Creature>& attackers, UIElement* defenderDisplay, UIElement* attackerDisplay){
	int pos = 0;
	for(auto blocker = defenders.begin(); blocker != defenders.end(); blocker++){
		if(blocker->get_flags()&1){ //untapped
			int y, z;
			defenderDisplay->get_coordinates(pos, &y, &z);
			Creature* evilguy = blocker_switch(*blocker, y, z, attackers, attackerDisplay);
			if(evilguy){
				blocker->set_blocking();
				evilguy->add_blocker(&(*blocker));
			}
		}
		pos++;
	}
}

Creature* Default_ui::blocker_switch(const Creature& blocker, int blockerY, int blockerZ, PContainer<Creature>& attackers, UIElement* attacker_io) const {
	int pos_evilguy = 0;
	blocker.disp(Rect(blockerY, blockerZ, permanentYSize, permanentZSize), true);
	auto evilguy = attackers.end();
	while(1){ //getting creature to block
		DirectioL dir = myIO->get_direction_key();
		if(dir == BACK || dir == ENTER){
			if(evilguy != attackers.end()) return &(*evilguy);
			else return 0;
		}
		else if(evilguy != attackers.end()){
			int y, z;
			attacker_io->get_coordinates(pos_evilguy, &y, &z);
			evilguy->disp(Rect(y, z, permanentYSize, permanentZSize), false); //displaying villain normally
			switch(dir){
				case DOWN: evilguy = attackers.end(); break;
				case LEFT:
					if(evilguy != attackers.begin()){
						--pos_evilguy;
						--evilguy;
					} break;
				case RIGHT:
					++evilguy;
					if(evilguy == attackers.end()) --evilguy;
					else pos_evilguy++;
					break;
				default: break;
			}
		}
		else if(dir == UP){
			evilguy = attackers.begin();
			pos_evilguy = 0;
			blocker.disp(Rect(blockerY, blockerZ, permanentYSize, permanentZSize), false); //displaying blocker normally
		}
		if(evilguy != attackers.end()){
			int y, z;
			attacker_io->get_coordinates(pos_evilguy, &y, &z);
			evilguy->disp(Rect(y, z, permanentYSize, permanentZSize), true); //displaying villain
		}
		else{
			blocker.disp(Rect(blockerY, blockerZ, permanentYSize, permanentZSize), true);
		}
	}
}