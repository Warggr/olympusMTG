#include "../HFiles/olympus_main.h"

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
	god.myIO->refresh_display();
}

void Game::disp_preRes(const PreResolvable* preRes, const std::string& origin_name) const {
	int y, z, yOffset, zOffset, ySize, zSize;
	stack_ui->get_coordinates(&y, &z, &yOffset, &zOffset, &ySize, &zSize);
	preRes->disp(y-yOffset, z-zOffset, ySize, zSize, origin_name);
}

void Game::disp_stack() const {
	int y, z, yOffset, zOffset, ySize, zSize;
	stack_ui->get_coordinates(&y, &z, &yOffset, &zOffset, &ySize, &zSize);
	stack_ui->erase_background(god.myIO);
	int pos = 0;
	for(std::forward_list<Resolvable*>::const_iterator iter = stack.cbegin(); iter!=stack.cend(); iter++){
		(*iter)->disp(y+pos*yOffset, z+pos*zOffset, ySize, zSize);
		pos++;
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
	int y = 0, z = 0, width = 0, height=0, liby=0, libz=0, gravey=0, gravez=0, exily=0, exilz=0;
	god.myUI->get_player_coords(player_id, &y, &z, &width, &height, &liby, &libz, &gravey, &gravez, &exily, &exilz);
	god.myIO->disp_header(y, z, width, height, name, life, state >> 5, highlight, manapool);
	
	myzones[0].disp(liby, libz);
	myzones[1].disp(gravey, gravez);
	myzones[2].disp(exily, exilz);
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
			if(!mylands.empty()) permUI[0]->disp_all<>(mylands);
			break;
		case 1:
			if(!myartos.empty()) permUI[1]->disp_all<>(myartos);
			break;
		case 2:
			if(!mysuperfriends.empty()) permUI[2]->disp_all<>(mysuperfriends);
			break;
		case 3:
			if(!mycreas.empty()) permUI[3]->disp_all<>(mycreas);
			break;
		case 4:
			if(!myattackers.empty()) permUI[4]->disp_all<>(myattackers);
			break;
	}
}

void CardZone::disp(int y, int z) const {
	god.myIO->disp_cardback(y, z);
	god.myIO->print_text(std::to_string(size), 0, y+20, z+20);
}