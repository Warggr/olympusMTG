void Game::disp_log(){
	wclear(winzones[0]);
	for(int i=0; i<LOGLEN && logbook[i] != 0; i++){
		mvwprintw(winzones[0], i, 0, "%s", logbook[i]);
	}
	wrefresh(winzones[0]);
}

void Game::deletegraph(){
	for(int i=0; i<3; i++) delwin(winzones[i]);
	endwin();
}

void Game::disp(){
	for(int i = 0; i<2; i++){
		players[i]->disp();
	}
	disp_log();
	disp_stack();
}

void Game::disp_stack(){
	wclear(winzones[1]);
	for(Resolvable* iter = stack; iter!=0; iter = iter->next){
		iter->describe(winzones[1]);
	}
	wrefresh(winzones[1]);
}

void Game::addtolog(const char* new_entry){
	if(logbook[LOGLEN -1]) delete logbook[LOGLEN -1];
	for(int i = LOGLEN-1; i>0; i--){
		logbook[i] = logbook[i-1];
	}
	char* r = new char[LEFTBAR_Y];
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

void Player::disp_header(){
	mvwprintw(winzones[0], 0, 0, " <%s> (%d life)  ", name, life);
	const char* phases_dcp[] = {"<\\ ", "\\a\\", "u=>", "<=t", ">*<", "/a/", " />"};
	char tmp[64];
	wprintw(winzones[0], "%s", m2t(manapool, tmp));
	for(int i=0; i<3; i++){
		myzones[i].describe(winzones[0], i*20);
	}
	char currentphase = state >> 5; mvwprintw(winzones[0], 0, 27, "%d", currentphase);
	wattron(winzones[0], A_ALTCHARSET);
	for(int i=0; i<7; i++){
		if(i==currentphase) wattron(winzones[0], A_REVERSE);
		mvwprintw(winzones[0], 0, 30+4*i, "%s", phases_dcp[i]);
		wattroff(winzones[0], A_REVERSE);
	}
	wattroff(winzones[0], A_ALTCHARSET);
	wrefresh(winzones[0]);
}

void Player::disp(){
	disp_header();
	for(int j=0; j<5; j++){
		disp_zone(j);
	}
	wrefresh(winzones[6]);
	wrefresh(winzones[7]);
}

void Player::disp_zone(int nbzone){
	wclear(winzones[nbzone+1]);
	int pos=0;
	for(Permanent* p = fperm[nbzone]; p!=0; p=p->next){
		p->disp(winzones[nbzone+1], CARD_Y*(pos++));
	}
	wrefresh(winzones[nbzone+1]);
}

void CardZone::describe(WINDOW* w, int y){
	mvwprintw(w, 1, y, "%s (%d)", name, size);
}

void Player::clear_opts(){
	wclear(winzones[6]);
	wrefresh(winzones[6]);
}