#include ".header_link.h"
#include "../Yggdrasil/headB_board.h"
#include "../HFiles/12abilities.h"

void Game::disp_log() const {
	int y, z, yOffset, zOffset;
	logbook_ui->get_coordinates(&y, &z, &yOffset, &zOffset);
	logbook_ui->erase_background(god.myIO);
	for(int i=0; i<LOGLEN && logbook[i] != 0; i++){
		god.myIO->print_text(logbook[i], 1, y+i*yOffset, z+i*zOffset);
	}
}

void Game::disp() const {
	god.myIO->fulldisp();
	for(int i = 0; i<2; i++){
		players[i]->disp();
	}
	disp_log();
	disp_stack();
	//god.myIO->refresh_display();
}

void Game::disp_preRes(const PreResolvable* preRes, const std::string& origin_name) const {
	int yOffset, zOffset;
	Rect stack = stack_ui->get_coordinates(&yOffset, &zOffset);
	stack.shift(-yOffset, -zOffset);
	preRes->disp(stack, origin_name);
}

void Game::disp_stack() const {
	int yOffset, zOffset;
	Rect rect = stack_ui->get_coordinates(&yOffset, &zOffset);
	stack_ui->erase_background(god.myIO);
	for(auto iter = stack.cbegin(); iter!=stack.cend(); iter++){
		(*iter)->disp(rect);
		rect.shift(yOffset, zOffset);
	}
}

#define LEFTBAR_Y 30

void Game::addtolog(const char* new_entry){
	if(logbook[LOGLEN -1]) delete[] logbook[LOGLEN -1];
	for(int i = LOGLEN-1; i>0; i--){
		logbook[i] = logbook[i-1];
	}
	char* r = new char[LEFTBAR_Y+1];
	bool v = true;
	for(int i=0; i<LEFTBAR_Y -1; i++){
		if(v) r[i] = new_entry[i];
		else r[i] = ' ';
		if(r[i] == '\0') v = false;
	}
	r[LEFTBAR_Y] = 0;
	logbook[0] = r;
	disp_log();
}

#undef LEFTBAR_Y

void Player::disp_header(bool highlight) const {
	int liby=0, libz=0, gravey=0, gravez=0, exily=0, exilz=0, cardzonewidth=0, cardzoneheight = 0;
	Rect zone;
	god.myUI->get_player_coords(player_id, &zone, &liby, &libz, &gravey, &gravez, &exily, &exilz, &cardzonewidth, &cardzoneheight);
	
	myzones[0].disp(Rect(liby, libz, cardzonewidth, cardzoneheight));
	myzones[1].disp(Rect(gravey, gravez, cardzonewidth, cardzoneheight));
	myzones[2].disp(Rect(exily, exilz, cardzonewidth, cardzoneheight));
	god.myIO->disp_header(zone, &(name[0]), life, state >> 5, highlight, manapool); //done last so NCurses knows to refresh the zone only then
}

void Player::disp() const {
	disp_header();
	for(int j=0; j<5; j++){
		disp_zone(j);
	}
}

void Player::disp_zone(int nbzone) const {
	switch(nbzone){
		case 0:
			permUI[0]->disp_all<>(&myboard.mylands);
			break;
		case 1:
			permUI[1]->disp_all<>(&myboard.myartos);
			break;
		case 2:
			permUI[2]->disp_all<>(&myboard.mysuperfriends);
			break;
		case 3:
			permUI[3]->disp_all<>(&myboard.mycreas); //TODO display only non-attacking creas
			break;
		case 4:
			if(myboard.myattackers) permUI[4]->disp_all<>(myboard.myattackers);
			break;
	}
}

void CardZone::disp(const Rect& zone) const {
	god.myIO->disp_cardback(zone, size);
}