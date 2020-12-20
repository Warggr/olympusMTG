#include "../HFiles/olympus_main.h"

_UIElement::_UIElement(int left, int top, int _ySize, int _zSize, int offset, int _maxItems, bool direction):
topZ(top), leftY(left), yOffset(0), zOffset(0), ySize(_ySize), zSize(_zSize), maxItems(_maxItems) {
	if(direction == vertical) zOffset = offset;
	else yOffset = offset;
	if(offset < 0){ //reversed zone
		topZ += zOffset;
		leftY += yOffset;
	}
	std::cout << "Dimensions: -" << left <<"-"<< left + maxItems*yOffset + ySize <<"-,|"<< top <<"|"<< top + maxItems*zOffset + zSize << std::endl;
}

UIElement* Default_ui::get_optionzone(){
	return optionZone;
}

Default_ui::Default_ui(Abstract_io* IOLib): Abstract_ui(IOLib){
	playerPerms[0] = new _UIElement(0, playerZ, boardY, permanentZSize, permanentZSize+permanentZMargin, 5, _UIElement::vertical);
	playerPerms[1] = new _UIElement(0, playerZ + 10*permanentZSize - 15, boardY, permanentZSize, -permanentZSize-permanentZMargin, 5, _UIElement::vertical);
	int posterY;
	IOLib->getResolution(&posterY, &posterZ);
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