#include "lib3_ncursesIO.h"

WINDOW** winzones{0};

void disp_mana(WINDOW* win, Mana mana, int endy, int topz);
Abstract_io* new_IOLib(){return new Ncurses_io; }

void Ncurses_io::draw_full_rectangle(char color, const Rect& zone) const {
	WINDOW* win = winzones[zone.zone()];
	if(color & Abstract_io::HIGH1) wattrset(win, A_STANDOUT);
	if(color & Abstract_io::HIGH2) wattron(win, A_BOLD);
	for(int i=0; i<zone.height; i++){
		wmove(win, zone.z+i, zone.y);
		for(int j=0; j<zone.width; j++)
			printw(" ");
	}
	wattroff(win, A_BOLD | A_STANDOUT);
}

void Ncurses_io::draw_rectangle(char color, const Rect& zone, int linewidth) const {
	WINDOW* win = winzones[zone.zone()];
	mvwprintw(win, zone.z, zone.yy(), "/"); for(int i=0; i<zone.width-2; i++) printw("-"); printw("\\");
	for(int i=0; i<zone.height-2; i++){
		mvwprintw(win, zone.z+i+1, zone.y, "|"); for(int i=0; i<zone.width-2; i++) printw("*"); printw("|");
	}
	mvwprintw(win, zone.bottom(), zone.y, "\\"); for(int i=0; i<zone.width-2; i++) printw("-"); printw("//");
}

void Ncurses_io::erase_surface(const Rect& zone) const {
	WINDOW* win = winzones[zone.zone()];
	if(zone.width == 0 && zone.height == 0) wclear(win);
	else{
		char* spaces = new char[zone.width +1];
		for(int i=0; i<zone.width; i++) spaces[i] = ' ';
		spaces[zone.width] = 0;
		for(int i=zone.z; i<zone.bottom(); i++){
			mvwprintw(win, i, zone.yy(), "%s", spaces);
		}
		delete[] spaces;
	}
}

void Ncurses_io::print_text(const char* text, char color, int y, int z) const {
    WINDOW* win = winzones[y >> 11]; y = y & 0x7ff;
    mvwprintw(win, z, y, "%s", text);
}

void Ncurses_io::draw_boxed_text(const char* text, char color, char backgr_color, int y, int z, int width) const {
	WINDOW* win = winzones[y >> 11]; y = y & 0x7ff;
	if(color & Abstract_io::HIGH2) wattron(win, A_BOLD);
	if(color & Abstract_io::HIGH1) wattron(win, A_STANDOUT);
	mvwprintw(win, y, z, "/");
	for(int i=0; i<width-2; i++) wprintw(win, "-");
	wprintw(win, "\\");
	mvwprintw(win, y+1, z, "|");
	for(int i=0; i<width-2; i++) wprintw(win, "%c", text[i]);
	wprintw(win, "|");
	wattroff(win, A_BOLD | A_STANDOUT);
}

void Ncurses_io::message(const char* text) const {
	mvwprintw(message_zone, 0, 0, "%s", text);
}

void Ncurses_io::draw_permanent(const Rect& zone, char color, bool tapped, bool highlight, bool basicImg) const {
	WINDOW* win = winzones[zone.zone()];
	wattron(win, COLOR_PAIR(color+1));
	char sep = tapped ? '/' : '|';
	if(highlight) wattron(win, A_STANDOUT);

	mvwprintw(win, zone.z, zone.yy(), "%c", sep); mvwprintw(win, zone.z, zone.yy()+zone.width, "%c", sep);
	mvwprintw(win, zone.z+1, zone.yy(), "%c", sep); for(int i=0; i<zone.width-2; i++) wprintw(win, " "); wprintw(win, "%c", sep);
	wmove(win, zone.z+2, zone.yy()); for(int i=0; i<zone.width; i++) wprintw(win, "-");

	wattroff(win, COLOR_PAIR(color+1));
	wattroff(win, A_STANDOUT);
}

void Ncurses_io::disp_header(const Rect& zone, const char* name, int life, char state, bool highlight, Mana pool) const {
	WINDOW* win = winzones[zone.zone()];
	if(highlight) wattron(win, A_STANDOUT);
	if(life >= 1000) raise_error("Life total too high to be shown, most likely a bug");
	mvwprintw(win, zone.yy(), zone.z, " <%s> (%d life)  ", name, life);

	if(zone.height != 1) disp_mana(win, pool, zone.right(), zone.bottom()-1);

	const char* phases_dcp[] = {"<\\ ", "\\a\\", "u=>", "<=t", ">*<", "/a/", " />"};
	wattron(winzones[0], A_ALTCHARSET);
	for(int i=0; i<7; i++){
		if(i==state >> 5) wattron(win, A_REVERSE);
		mvwprintw(win, zone.z, zone.right() - 4*(7-i), "%s", phases_dcp[i]);
		if(i==state >> 5) wattroff(win, A_REVERSE);
	}
	wattroff(win, A_ALTCHARSET);
	wrefresh(win);
}

void Ncurses_io::disp_cardback(const Rect& zone, int oncard_number) const {
	WINDOW* win = winzones[zone.zone()];
	if(zone.width >= 7 && zone.height >= 3){
		mvwprintw(win, zone.z, zone.yy(),   "/MAGIC\\");
		mvwprintw(win, zone.z+1, zone.yy(), "|*%3d*|", oncard_number);
		mvwprintw(win, zone.z+2, zone.yy(), "\\-----/");
	}
	else{
		mvwprintw(win, zone.z, zone.yy(), "/"); printw("%3d", oncard_number); for(int i=0; i<zone.width-5; i++) printw( "-"); printw( "\\");
		for(int i=0; i<zone.height-2; i++){
			mvwprintw(win, zone.z+i+1, zone.yy(), "|"); for(int j=0; j<zone.width-5; j++) printw( "*"); printw( "|");
		}
		mvwprintw(win, zone.z+zone.height, zone.yy(), "\\"); for(int i=0; i<zone.width-2; i++) printw( "-"); printw( "/");
	}
}

void Ncurses_io::poster(const std::string name, Mana manacost, char color, const char* types,
	const std::vector<std::string> lines, int power, int toughness, char frametype, bool watermark) const {
	WINDOW* win = winzones[POSTER];
	wclear(win);
	wattron(win, COLOR_PAIR(color+1));
	wmove(win, 0, 0);
	for(int i=0; i<RIGHTBAR_Y; i++) wprintw(win, "-");
	wmove(win, FULLCARD_Z -1, 0);
	for(int i=0; i<RIGHTBAR_Y; i++) wprintw(win, "-");
	mvwprintw(win, 2, 2, "%s", &(name[0]));

	disp_mana(win, manacost, RIGHTBAR_Y-1, 2);
	mvwprintw(win, FULLCARD_Z/2, 1, "%s", types);
	if(watermark){
		wattron(win, A_ALTCHARSET);
		for(int i=0; i<3; i++){
			mvwprintw(win, FULLCARD_Z/2 + 3 + i, RIGHTBAR_Y/2-1, "%s", manasymbols + color*12 + i*4);
		}
		wattroff(win, A_ALTCHARSET);
	}
	for(int i=0; ((unsigned)i) <lines.size() && i + FULLCARD_Z/2 + 1 != FULLCARD_Z -2; i++){
		mvwprintw(win, i + FULLCARD_Z/2 + 1, 0, "%s", &(lines[i][0]));
	}
	if(frametype == 1){
		char x[10]; sprintf(x, "(%d/%d)", power, toughness);
		mvwprintw(win, FULLCARD_Z-1, RIGHTBAR_Y - 5, "%s", x);
	}
	else if(frametype == 2){
		char x[5]; sprintf(x, "\\%d/", power);
		mvwprintw(win, FULLCARD_Z-1, RIGHTBAR_Y - 4, "%s", x);
	}
	wattroff(win, COLOR_PAIR(color+1));
	wrefresh(win);
}

void disp_mana(WINDOW* win, Mana mana, int endy, int topz){
	const char* color_sym = "CWUBRG";
	int m = mana.m2i();
	if(m == 0) return;
	char generic = (char) (m&0xf);
	char display[generic];
	for(int i=0; i<6; i++){
		m = m >> 4;
		for(int j=0; j < (m&0xf); j++){
			generic--;
			display[generic -j-1] = color_sym[i];
		}
	}
	if(generic >= 10) mvwprintw(win, endy-generic-2, topz, "%d%s", generic, display);
	else if(generic!=0) mvwprintw(win, endy-generic-1, topz, "%d%s", generic, display);
	else mvwprintw(win, endy-generic, topz, "%s", display);
}

DirectioL Ncurses_io::get_direction_key(){
	while(1){
		int ch;
		ch = getch();
		mouseActive = false;
		switch(ch){
			case KEY_DOWN: return DOWN;
			case KEY_UP: return UP;
			case 32: return ENTER; //ENTER
			case 10: return BACK; //SPACE
			case KEY_RIGHT: return RIGHT;
			case KEY_LEFT: return LEFT;
			// case ALLEGRO_EVENT_DISPLAY_CLOSE:
			// 	god.call_ragnarok();
			case KEY_MOUSE:
				MEVENT event;
				if(getmouse(&event) == OK){
					mouseActive = true;
					if(event.bstate & REPORT_MOUSE_POSITION){
						mousey = event.x; mousez = event.y;
					}
					if(event.bstate & BUTTON1_CLICKED) return ENTER;
					else return MOUSE;
				} //fallthrough -> unrecognized key
			default: break;
		}
	}
}

int Ncurses_io::getInt(int lowerBound, int upperBound){
	if(lowerBound >= upperBound) return lowerBound;
	int ret = lowerBound;
	mvwprintw(message_zone, 0, 0, "Please enter a number between %d and %d:", lowerBound, upperBound);
	while(1){
		mvwscanw(message_zone, 0, 1, "%d", &ret);
		if(ret >= lowerBound && ret <= upperBound) return ret;
	}
}

WINDOW* newwin(const Rect& zone){
	return newwin(zone.width, zone.height, zone.y, zone.z);
}

Ncurses_io::Ncurses_io(){
	initscr();
	noecho(); //don't show typed chars
	keypad(stdscr, TRUE); //capture backspace, delete, arrow keys

	start_color();

	//init_color(COLOR_RED, 700, 0, 0)
	init_pair(1, COLOR_BLACK, COLOR_CYAN); //artifacts
	init_pair(2, COLOR_BLACK, COLOR_YELLOW); //Peace through Order
	init_pair(3, COLOR_CYAN, COLOR_BLUE); //Perfection through Knowledge
	init_pair(4, COLOR_MAGENTA, COLOR_BLACK); //Power through Ambition
	init_pair(5, COLOR_BLACK, COLOR_RED); //Freedom through Action
	init_pair(6, COLOR_BLACK, COLOR_GREEN); //Growth through Harmony
	init_pair(7, COLOR_YELLOW, COLOR_BLACK); //Multicolor

	mousemask(REPORT_MOUSE_POSITION | BUTTON1_CLICKED, 0);
}

void Ncurses_io::getResolution(int& y, int& z, bool& hasMouseSupport, int& linesize) const {
	y = COLS;
	z = LINES;
	hasMouseSupport = true;
	linesize = 1;
}

void Ncurses_io::harmonize(const Rect& message, const Rect& poster, int nb_winzones){
	message_zone = newwin(message); poster_zone = newwin(poster);
	winzones = new WINDOW*[nb_winzones];
	box(poster_zone, 0, 0);
	wrefresh(poster_zone);
	wgetch(poster_zone);
}

void Ncurses_io::declare_window(int& leftY, int& topz, int width, int height){
	winzones[nb_winzones] = newwin(height, width, topz, leftY);
	box(winzones[nb_winzones], 0, 0);
	mvwprintw(winzones[nb_winzones], 0, 0, "%dx%d", leftY, topz);
	wrefresh(winzones[nb_winzones]);
	god.gdebug(DBG_IOUI) << "Created zone " << leftY << ", " << topz << ", " << width << ", " << height << "\n";
	leftY = nb_winzones << 11;
	topz = 0;
	nb_winzones++;
}

Ncurses_io::~Ncurses_io(){
	if(winzones != 0){
		for(int i=0; i<nb_winzones; i++) delwin(winzones[i]);
		delete[] winzones;
	}
	endwin();
}