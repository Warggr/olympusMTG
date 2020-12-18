#include "../HFiles/olympus_main.h"

void upperframe(WINDOW* win, int x, int y_size){
	mvwprintw(win, x, 0, "/");
	for(int i=0; i<y_size-2; i++) wprintw(win, "-");
	wprintw(win, "\\");
	mvwprintw(win, x+1, 0, "|");
	mvwprintw(win, x+1, y_size-1, "|");
}

void Ncurses_io::disp_mana(const Mana m, int endy, int topz){
	char generic = (char) (m&0xf);
	for(int i=0; i<6; i++){
		m = m >> 4;
		for(int j=0; j < (m&0xf); j++){
			endy -= 24;
			generic--;
			al_draw_scaled_bitmap(images->ManaColorSym[i], 0, 0, 160, 160, endy, topz, 24, 24, 0);
		}
	}
	if(generic!=0){
		al_draw_scaled_bitmap(images->ManaNumSym[(int) generic], 0, 0, 160, 160, endy-24, topz, 24, 24, 0);
	}
}

void Ncurses_io::poster(int y, int z,
	const std::string name, const Mana manacost, char color, const std::string types,
	int nb_lines, const std::string* lines, int power, char frametype, int toughness, bool watermark){
	
	wclear(win);
	wattron(win, COLOR_PAIR(color+1));
	const char* nametypes[] = {"Instant", "Land", "Artifact", "Planeswalker", "Creature", "Sorcery"};
	wmove(win, 0, 0);
	for(int i=0; i<RIGHTBAR_Y; i++) wprintw(win, "-");
	wmove(win, FULLCARD_X -1, 0);
	for(int i=0; i<RIGHTBAR_Y; i++) wprintw(win, "-");
	mvwprintw(win, 2, 2, "%s", name, color);
	char tmp[64];
	mvwprintw(win, 2, RIGHTBAR_Y-6, "%s", m2t(cost, tmp));
	mvwprintw(win, FULLCARD_X/2, 1, "%s", nametypes[(int) type]);
	if(type == 1){
		wattron(win, A_ALTCHARSET);
		for(int i=0; i<3; i++){
			mvwprintw(win, FULLCARD_X/2 + 3 + i, RIGHTBAR_Y/2-1, "%s", manasymbols + color*12 + i*4);
		}
		wattroff(win, A_ALTCHARSET);
	}
	if(fab) fab->describe(win, 0, RIGHTBAR_Y-2, FULLCARD_X /2 +2, name);
	for(int i=0; i<nb_actabs; i++){
		first_actab[i].describe(win, RIGHTBAR_Y-3, FULLCARD_X/2 + 5 + i, name);
	}
	if(flavor_text) mvwprintw(win, FULLCARD_X/2 + 7, 1, "%s", flavor_text);
	wattroff(win, COLOR_PAIR(color+1));
	wrefresh(win);

	al_draw_bitmap(card_template[(int) color], z, y, 0);
	al_draw_text(font, al_map_rgb(0,0,0), y+29, z+28, 0, &(name[0]));
	disp_mana(cost, y - 28, z + 30);
	al_draw_text(font, al_map_rgb(0,0,0), y+29, z+295, 0, &(types[0]));
	if(watermark){
		al_draw_bitmap(watermarks[(int) color], y + 120, z + 340, 0);
	}
	for(int i=0; i<nb_lines; i++){
		al_draw_text(font, al_map_rgb(0,0,0), y+29, z+360+ i*15);
	}
	const int yOffset = 60;
	const int zOffset = 85;
	if(frametype == 1){
		al_draw_bitmap(pt_box[(int) color], y+fullcardY-yOffset, z+fullcardZ-zOffset, 0);
		char x[10]; sprintf(x, "%d/%d", power, toughness);
		al_draw_text(font, al_map_rgb(0, 0, 0), y+fullcardY-yOffset + 20, z+fullcardZ-zOffset + 5, 0, x);
	}
	else if(frametype == 2){
		al_draw_bitmap(loyalty, y+fullcardY-yOffset, z+fullcardZ-zOffset, 0);
		char x[3]; sprintf(x, "%d", power);
		al_draw_text(font, al_map_rgb(255, 255, 255), y+fullcardY-yOffset + 30, z+fullcardZ-zOffset + 8, 0, x);
	}
}