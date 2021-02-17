#include "lib3_ncursesIO.h"

bool Ncurses_io::attack_switch(int leftY, int Ywidth, int topZ, int arrowlength) const {
	bool attacks = false;
	int nbzone = leftY & 0xf; leftY = leftY >> 4;
	char* arrows = new char[Ywidth+1]; char* clear = new char[Ywidth+1];
	char arrow = arrowlength > 0 ? 'V' : 'A';
	for(int i=0; i<Ywidth; i++){
		arrows[i] = arrow; clear[i] = ' ';
	}
	arrows[Ywidth] = 0; clear[Ywidth] = 0;
	while(1){
		wrefresh(winzones[nbzone]);
		int ch;
		ch = getch();
		switch(ch){
			case KEY_UP:
				attacks = true;
				mvwprintw(winzones[nbzone], topZ, leftY, "%s", arrows);
				message("[ATTACKS]");
				break;
			case KEY_DOWN:
				attacks = false;
				mvwprintw(winzones[nbzone], topZ, leftY, "%s", clear);
				message("[STAYS]");
				break;
			case KEY_RIGHT:
			case 32:
			case 10:
				delete[] clear;
				delete[] arrows;
				return attacks;
			default: break;
		}
	}
}