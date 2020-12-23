#include <cstdio>
#include <cstdlib>
#include "HFiles/classes.h"

char manasymbols[] = " - \0,O+\0 . \0mXl\0qOq\0jXk\0 k \0lj)\0(sj\0lqk\0OwO\0 - \0 k-\0o u\0(gz\0(Ok\0cO)\0 a";

void Card::describe(WINDOW* win){
	wprintw(win, "%s", name);
	if(type != 1){
		char tmp[64];
		wprintw(win, " - %s", m2t(cost, tmp));
	}
	wprintw(win, "\n");
}

void PermOption::describe(WINDOW* win, int maxlen, int x, const char* name){
	char tmp[64];
	mvwprintw(win, x, 1, "%s", m2t(cost, tmp));
	if(tapsymbol) wprintw(win, ", T");
	wprintw(win, ": ");
	fab->describe(win, 6, maxlen-3, x, name);
}

void SpellOption::disp_highlighted(WINDOW* win, int pos, WINDOW* posterzone, bool castable){
	if(posterzone) source->poster(posterzone);
	disp(win, pos, true, castable);
}

void SpellOption::disp(WINDOW* win, int pos, bool highlight, bool castable){
	if(highlight) wattron(win, A_STANDOUT);
	if(castable) wattron(win, A_BOLD);
	upperframe(win, pos, 40);
	wmove(win, pos+1, 1);
	source->describe(win);
	wattroff(win, A_STANDOUT | A_BOLD);
}

void Permanent::describe(WINDOW* w){ wprintw(w, "%s", name);}

void Creature::describe(WINDOW* w){ wprintw(w, "%s: %d/%d", name, power, toughness);}

void Permanent::disp(WINDOW* win, int y, WINDOW* posterzone){
	wattron(win, A_STANDOUT);
	source->poster(posterzone);
	disp(win, y);
	wattroff(win, A_STANDOUT);
}

void falsePermanentDisp(WINDOW* win, int y){
	for(int i=0; i<CARD_X; i++){
		wmove(win, i, y);
		for(int i=0; i<CARD_Y; i++){
			wprintw(win, " ");
		}
	}
}

void Permanent::disp(WINDOW* win, int y){
	wattron(win, COLOR_PAIR(color+1));
	char sep = '/';
	if(flags&1) sep = '|';
	mvwprintw(win, 0, y, "%c", sep);
	for(int i=0;i<CARD_Y-2 && name[i]!=0;i++) wprintw(win, "%c", name[i]);
	wprintw(win, "%c", sep);
	mvwprintw(win, 1,y, "%c", sep); for(int i=0; i<CARD_Y-2; i++) wprintw(win, " ");
	wprintw(win, "%c", sep);
	wmove(win, 2, y); for(int i=0; i<CARD_Y; i++) wprintw(win, "-");
}

void Creature::disp(WINDOW* win, int y){
	Permanent::disp(win, y);
	mvwprintw(win, 2, y + 7, "(%d/%d)", power, toughness);
}

void Ability::describe(WINDOW* win, int miny, int maxlen, int x, const char* cardname){
	wmove(win, x, miny);
	const char* descriptions[NB_DIFFERENT_ABILITIES];
	descriptions[0] = "~ deals ? damage to target opponent";
	descriptions[1] = "Target player draws ? cards";
	descriptions[2] = "Target player gains ? life";
	descriptions[3] = "Your life total becomes ?";
	descriptions[4] = "Add one mana of color ?";
	descriptions[5] = "Destroy something";
	descriptions[6] = "Counter target spell or ability.";
	const char* ds = descriptions[(int) type];
	int i=0;
	while(1){
		char a = *(ds++);
		if(a == '~'){
			int k; for(k = 0; cardname[k] != 0; k++){
				wprintw(win, "%c", cardname[k]);
				i++; if(i==maxlen){
					i=0;
					x++;
					wmove(win, x, 1);
				}
			}
		}
		else if(a == '?'){
			wprintw(win, "%d", param2);
			i++; if(param2>10) i++; //Well there's a problem if param if 100 or more, but that's not a concern right now
		}
		else if(a == '\0'){
			if(next) next->describe(win, miny, maxlen, x+1, cardname);
			return;
		}
		else{
			wprintw(win, "%c", a); i++;
		} 
		if(i==maxlen){
			i=0;
			x++;
			wmove(win, x, miny);
		}
	}
}