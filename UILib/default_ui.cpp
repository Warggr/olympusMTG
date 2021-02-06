#include "12defaultUI.h"

Abstract_ui* new_UILib(Abstract_io* IOLib){return new Default_ui(IOLib); }

void Default_ui::normalize_gridy_gridz(){
	if(myIO->gmouseY() < boardY){
		gridy = 0;
		if(myIO->gmouseZ() < playerZ) gridz = 0;
		else if(myIO->gmouseZ() > playerZ + 10*permanentZSize - 15) gridz = 11;
		else gridz = (myIO->gmouseZ() - playerZ) / (permanentZSize+permanentZMargin) - 1;
	}
	else if(myIO->gmouseY() < boardY + leftbarY){
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
	//std::cout << "Dimensions: -" << left <<"-"<< left + maxItems*yOffset + ySize <<"-,|"<< top <<"|"<< top + maxItems*zOffset + zSize << std::endl;
}

UIElement* Default_ui::get_optionzone(){
	return optionZone;
}

Default_ui::Default_ui(Abstract_io* IOLib): Abstract_ui(IOLib){
	playerPerms[0] = new _UIElement(0, playerZ, boardY, permanentZSize, permanentZSize+permanentZMargin, 5, _UIElement::vertical);
	playerPerms[1] = new _UIElement(0, playerZ + 10*permanentZSize - 15, boardY, permanentZSize, -permanentZSize-permanentZMargin, 5, _UIElement::vertical);
	int posterY;
	IOLib->getResolution(posterY, posterZ, mouseSupport);
	IOLib->harmonize(boardY, 0, boardY+leftbarY, optionsZ); //posterY - posterZ - messageY - messageZ
	optionZone = new _UIElement(boardY + leftbarY, 0, rightbarY, optionZSize, optionZSize+5, 10, _UIElement::vertical);
	stackZone = new _UIElement(boardY, posterZ + optionZSize, rightbarY, optionZSize, optionZSize+2, 10, _UIElement::vertical); //there must be an upper-margin over the stack, for PreRes
	logbookZone = new _UIElement(boardY + leftbarY, stackZ+iozZ, rightbarY, optionZSize, optionZSize+2, LOGLEN, _UIElement::vertical);
	for(int i=0; i<2; i++){
		for(int j=0; j<5; j++){
			int y, z;
			playerPerms[i]->get_coordinates(j, &y, &z);
			permanentZones[i*5 + j] = new _UIElement(y, z, permanentYSize, permanentZSize, permanentYSize+5, boardY/(permanentYSize+5), _UIElement::horizontal);
		}
	}
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

void _UIElement::get_coordinates(int* y, int* z, int* yOff, int* zOff, int* yWidth, int* zHeight) const {
	*y = leftY;
	*z = topZ;
	*yOff = yOffset;
	*zOff = zOffset;
	*yWidth = ySize;
	*zHeight = zSize;
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
	io->erase_surface(leftY, topZ, (maxItems-1)*yOffset + ySize, (maxItems-1)*zOffset + zSize);
}

void Default_ui::clear_opts(){
	optionZone->erase_background(myIO);
}

void Default_ui::get_player_coords(char player_id, int* py, int* pz, int* wide, int* high, int* liby, int* libz, int* gravey, int* gravez, int* exily, int* exilz){
	*gravey = 5;
	*liby = 55;
	*exily = 105;
	*py = 120;
	*wide = boardY - *py;
	*high = playerZ;
	if(player_id == 1){
		*gravez = 5;
		*pz = 0;
	}
	else{
		*gravez = playerZ + 10*permanentZSize -20;
		*pz = playerZ + 10*permanentZSize -20;
	}
	*libz = *gravez;
	*exilz = *gravez;
}

bool Default_ui::chooseattackers(PContainer<Creature>& cowards, PContainer<Creature>& warriors, char player_id){
	myIO->message("Choose Attackers");
	bool ret = false;
	int i = 0;
	int y, z, yOff, zOff, ySize, zSize;
	permanentZones[(player_id-1) * 5 + 3]->get_coordinates(&y, &z, &yOff, &zOff, &ySize, &zSize);
	for(auto iter = cowards.begin(); iter != cowards.end(); i++){
		if((iter->get_flags()&3) == 3){ //untapped and no summoning sickness
			int zPos = (player_id == 1) ? z + i*zOff + zSize : z + i*zOff; //getting either top or bottom Z
			bool attacks = myIO->attack_switch(y + i*yOff, ySize, zPos, (player_id == 1) ? 20 : -20);
			if(attacks){
				auto newatt = iter;
				newatt->disp(y+i*yOff, z+i*zOff, ySize, zSize, false); //disp creature normally
				newatt->reset_flags(1); //tap creature
				iter++;
				newatt.get_pointed()->unstate(); //move creature to list "warriors"
				ret = true;
			}
			else{
				iter->disp(y+i*yOff, z+i*zOff, ySize, zSize, false); //disp creature normally
				iter++;
			}
		}
		else iter++;
	}
	return ret;
}

void Default_ui::chooseblockers(PContainer<Creature>& defenders, PContainer<Creature>& attackers, UIElement* defenderDisplay, UIElement* attackerDisplay){
	int pos = 0;
	for(auto blocker = defenders.begin(); blocker != defenders.end(); blocker++){
		if(blocker->get_flags()&1){ //untapped
			int y, z;
			defenderDisplay->get_coordinates(pos, &y, &z);
			Creature* evilguy = myIO->blocker_switch(*blocker, y, z, attackers, attackerDisplay, permanentYSize, permanentZSize);
			if(evilguy){
				blocker->set_blocking();
				evilguy->add_blocker(&(*blocker));
			}
		}
		pos++;
	}
}

const int Default_ui::leftbarY = 400;
const int Default_ui::rightbarY = 400;
const int Default_ui::stackZ = 200;
const int Default_ui::optionsZ = 300;
const int Default_ui::playerZ = 70;
const int Default_ui::iozZ = 200;
const int Default_ui::permanentZSize = 80;
const int Default_ui::permanentYSize = 80;
const int Default_ui::boardY = (permanentYSize + 5) * 10;
const int Default_ui::optionZSize = 30;
const int Default_ui::permanentZMargin = 2;