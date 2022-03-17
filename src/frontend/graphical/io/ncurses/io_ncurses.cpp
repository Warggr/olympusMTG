#include "lib3_ncursesIO.h"
#include "gameplay/gamer.h"
#include "gameplay/permanents/4permanents.h"
#include "gameplay/resolvables/5resolvables.h"

void disp_mana(WINDOW* win, Mana mana, int endy, int topz);

void NcursesIO::draw_full_rectangle(char color, const Rect& zone) const {
	WINDOW* win = winzones[zone.zone];
	if(color & AbstractIO::HIGH1) wattrset(win, A_STANDOUT);
	if(color & AbstractIO::HIGH2) wattron(win, A_BOLD);
	for(int i=0; i<zone.height; i++){
		wmove(win, zone.z+i, zone.y);
		for(int j=0; j<zone.width; j++)
			printw(" ");
	}
	wattroff(win, A_BOLD | A_STANDOUT);
}

void NcursesIO::draw_rectangle(char, const Rect& zone, int) const {
	WINDOW* win = winzones[zone.zone];
	mvwprintw(win, zone.z, zone.y, "/"); for(int i=0; i<zone.width-2; i++) printw("-"); printw("\\");
	for(int i=0; i<zone.height-2; i++){
		mvwprintw(win, zone.z+i+1, zone.y, "|"); for(int j=0; j < zone.width - 2; j++) printw("*"); printw("|");
	}
	mvwprintw(win, zone.bottom(), zone.y, "\\"); for(int i=0; i<zone.width-2; i++) printw("-"); printw("//");
}

void NcursesIO::erase_surface(const Rect& zone) const {
	WINDOW* win = winzones[zone.zone];
	if(zone.width == 0 && zone.height == 0) wclear(win);
	else{
		char* spaces = new char[zone.width +1];
		for(int i=0; i<zone.width; i++) spaces[i] = ' ';
		spaces[zone.width] = 0;
		for(int i=zone.z; i<zone.bottom(); i++){
			mvwprintw(win, i, zone.y, "%s", spaces);
		}
		delete[] spaces;
	}
}

void NcursesIO::print_text(const char* text, char, int y, int z) const {
    WINDOW* win = winzones[y >> 11]; y = y & 0x7ff;
    mvwprintw(win, z, y, "%s", text);
}

void NcursesIO::draw_boxed_text(const char* text, char color, char, int y, int z, int width) const {
	WINDOW* win = winzones[y >> 11]; y = y & 0x7ff;
	if(color & AbstractIO::HIGH2) wattron(win, A_BOLD);
	if(color & AbstractIO::HIGH1) wattron(win, A_STANDOUT);
	mvwprintw(win, y, z, "/");
	for(int i=0; i<width-2; i++) wprintw(win, "-");
	wprintw(win, "\\");
	mvwprintw(win, y+1, z, "|");
	for(int i=0; i<width-2; i++) wprintw(win, "%c", text[i]);
	wprintw(win, "|");
	wattroff(win, A_BOLD | A_STANDOUT);
}

void NcursesIO::message(const char* text) const {
	mvwprintw(message_zone, 0, 0, "%s", text);
}

void NcursesIO::draw_permanent(const Rect& zone, const std::string& name, char color, bool tapped, bool highlight, bool) const {
	WINDOW* win = winzones[zone.zone];
	wattron(win, COLOR_PAIR(color+1));
	char sep = tapped ? '/' : '|';
	if(highlight) wattron(win, A_STANDOUT);

	mvwprintw(win, zone.z, zone.y, "%c", sep); wprintw(win, "%s", name.c_str());
	mvwprintw(win, zone.z, zone.y+zone.width, "%c", sep);
	mvwprintw(win, zone.z+1, zone.y, "%c", sep); for(int i=0; i<zone.width-2; i++) wprintw(win, " "); wprintw(win, "%c", sep);
	wmove(win, zone.z+2, zone.y); for(int i=0; i<zone.width; i++) wprintw(win, "-");

	wattroff(win, COLOR_PAIR(color+1));
	wattroff(win, A_STANDOUT);
}

void NcursesIO::draw(const Gamer& player, const Rect& zone, bool highlight) const {
	WINDOW* win = winzones[zone.zone];
	if(highlight) wattron(win, A_STANDOUT);
	assert(player.getLife() < 1000);
	mvwprintw(win, zone.y, zone.z, " <%s> (%d life)  ", player.getName().c_str(), player.getLife());

	if(zone.height != 1) disp_mana(win, player.manapool, zone.right(), zone.bottom()-1);

	const char* phases_dcp[] = {"<\\ ", "\\a\\", "u=>", "<=t", ">*<", "/a/", " />"};
	wattron(winzones[0], A_ALTCHARSET);
	for(int i=0; i<7; i++){
		if(i == player.getPhase()) wattron(win, A_REVERSE);
		mvwprintw(win, zone.z, zone.right() - 4*(7-i), "%s", phases_dcp[i]);
		if(i == player.getPhase()) wattroff(win, A_REVERSE);
	}
	wattroff(win, A_ALTCHARSET);
	wrefresh(win);
}

void NcursesIO::draw_cardback(const Rect& zone, int oncard_number) const {
	WINDOW* win = winzones[zone.zone];
	if(zone.width >= 7 && zone.height >= 3){
		mvwprintw(win, zone.z, zone.y,   "/MAGIC\\");
		mvwprintw(win, zone.z+1, zone.y, "|*%3d*|", oncard_number);
		mvwprintw(win, zone.z+2, zone.y, "\\-----/");
	}
	else{
		mvwprintw(win, zone.z, zone.y, "/"); printw("%3d", oncard_number); for(int i=0; i<zone.width-5; i++) printw( "-"); printw( "\\");
		for(int i=0; i<zone.height-2; i++){
			mvwprintw(win, zone.z+i+1, zone.y, "|"); for(int j=0; j<zone.width-5; j++) printw( "*"); printw( "|");
		}
		mvwprintw(win, zone.z+zone.height, zone.y, "\\"); for(int i=0; i<zone.width-2; i++) printw( "-"); printw( "/");
	}
}

void NcursesIO::poster(const CardOracle& card) const {
	draw(card, Rect(0, 0, POSTER, RIGHTBAR_Y, FULLCARD_Z), true);
}

void disp_mana(WINDOW* win, Mana mana, int endy, int topz){
	std::string m = mana.m2t();
	mvwprintw(win, endy-m.size(), topz, "%s", m.c_str());
}

DirectioL NcursesIO::get_direction_key() {
	while(true){
		int ch;
		ch = getch();
		mouseActive = false;
		message("Received key " + std::to_string(ch));
		gdebug(DBG_IOUI) << "Received key " << ch << "\n";
		switch(ch){
			case KEY_DOWN: return DOWN;
			case KEY_UP: return UP;
			case 32: return ENTER; //ENTER
			case 10: return BACK; //SPACE
			case KEY_RIGHT: return RIGHT;
			case KEY_LEFT: return LEFT;
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

int NcursesIO::getInt(int lowerBound, int upperBound){
	if(lowerBound >= upperBound) return lowerBound;
	int ret = lowerBound;
	mvwprintw(message_zone, 0, 0, "Please enter a number between %d and %d:", lowerBound, upperBound);
	while(true){
		mvwscanw(message_zone, 0, 1, "%d", &ret);
		if(ret >= lowerBound && ret <= upperBound) return ret;
	}
}

WINDOW* newwin(const Rect& zone){
	return newwin(zone.width, zone.height, zone.y, zone.z);
}

NcursesIO::NcursesIO(){
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

void NcursesIO::getResolution(int& y, int& z, int& linesize) const {
	y = COLS;
	z = LINES;
	linesize = 1;
}

void NcursesIO::harmonize(const Rect& message, const Rect& poster, int n_winzones){
	message_zone = newwin(message); poster_zone = newwin(poster);
	winzones = new WINDOW*[n_winzones];
	box(poster_zone, 0, 0);
	wrefresh(poster_zone);
	wgetch(poster_zone);
}

void NcursesIO::declare_window(int& leftY, int& topz, int width, int height){
	winzones[nb_winzones] = newwin(height, width, topz, leftY);
	box(winzones[nb_winzones], 0, 0);
	mvwprintw(winzones[nb_winzones], 0, 0, "%dx%d", leftY, topz);
	wrefresh(winzones[nb_winzones]);
	gdebug(DBG_IOUI) << "Created zone " << leftY << ", " << topz << ", " << width << ", " << height << "\n";
	leftY = nb_winzones << 11;
	topz = 0;
	nb_winzones++;
}

NcursesIO::~NcursesIO(){
	if(winzones != nullptr){
		for(int i=0; i<nb_winzones; i++) delwin(winzones[i]);
		delete[] winzones;
	}
	endwin();
}

void NcursesIO::disp_inrow(const Displayable* disp, int number, int total, int flags) const {
    (void) disp; (void) number; (void) total; (void) flags; //TODO
}

void NcursesIO::disp_player(const Gamer& player, int flags) const {
    (void) player; (void) flags;
    //TODO IMPLEM: what happens when you click on an opponent to get more details about him?
}

std::string NcursesIO::getTextInput(const char* question) {
    message(question);
    char buffer[1024];
    mvwscanw(message_zone, 0, 1, "%s", buffer);
    return std::string(buffer);
}

bool NcursesIO::simpleChoice(const char* optTrue, const char* optFalse) {
    message(optTrue);
    message(optFalse);
    char buffer;
    mvwscanw(message_zone, 0, 1, "%c", &buffer);
    if(buffer == 'y' || buffer == '1') return true;
    return false;
}

void NcursesIO::setMenuScene() {
    //TODO IMPLEM menu scene
}

void NcursesIO::setGameScene() {
    //TODO IMPLEM game scene
}

void NcursesIO::draw(const CardOracle& card, const Rect& zone, bool highlight) const {
    WINDOW* win = winzones[zone.zone];
    wclear(win);
    auto card_color = main_color(card.getCost().mana.m2color());

    wattron(win, COLOR_PAIR( card_color + 1));
    if(highlight) wattron(win, HIGHLIGHT);
    wmove(win, zone.z, zone.y);
    for(int i=zone.y; i<zone.right(); i++) wprintw(win, "-");
    wmove(win, zone.height-1, 0);
    for(int i=zone.y; i<zone.right(); i++) wprintw(win, "-");
    mvwprintw(win, zone.z+2, zone.y+2, "%s", card.getName().c_str());

    disp_mana(win, card.getCost().mana, zone.right()-1, zone.z+2);
    mvwprintw(win, zone.z + zone.height/2, zone.z+1, "%s", card.getType().toString().c_str());
    if(card.getType().land and card.getType().shift){
        wattron(win, A_ALTCHARSET);
        for(int i=0; i<3; i++){
            mvwprintw(win, zone.z + 3*zone.height/4 - 1 + i, zone.y + zone.width/2-1, "%s", manasymbols + card_color*12 + i*4);
        }
        wattroff(win, A_ALTCHARSET);
    }
    int i = 0;
    int frametype, power, toughness;
    for(auto& line : card.allText(power, toughness, frametype)){
        mvwprintw(win, i + zone.z + zone.height/2 + 1, zone.y, "%s", line.c_str());
        ++i;
        if(i + zone.z + zone.height/2 + 1 == zone.bottom()-2) break;
    }
    if(frametype == 1){
        char x[10]; sprintf(x, "(%d/%d)", power, toughness);
        mvwprintw(win, zone.bottom()-1, zone.right()-5, "%s", x);
    }
    else if(frametype == 2){
        char x[5]; sprintf(x, "\\%d/", power);
        mvwprintw(win, zone.bottom()-1, zone.right()-4, "%s", x);
    }
    wattroff(win, COLOR_PAIR(card_color+1));
    wattroff(win, HIGHLIGHT);
    wrefresh(win);
}

void NcursesIO::draw(const Resolvable& resolvable, const Rect& zone, bool highlight) const {
    if(highlight) wattron(winzones[zone.zone], HIGHLIGHT);
    draw_full_rectangle(main_color(resolvable.getColor()), zone);
    draw_boxed_text(resolvable.describe(), BLACK, main_color(resolvable.getColor()), zone.y, zone.z, zone.width);
    wattroff(winzones[zone.zone], HIGHLIGHT);
}

void NcursesIO::draw(const Permanent& perm, const Rect& zone, bool highlight) const {
    draw_permanent(zone, perm.getName(), main_color(perm.getManaCost().m2color()), !perm.isUntapped(), highlight, true);
}
