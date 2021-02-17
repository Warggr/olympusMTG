#include "lib3_nanoIO.h"

Abstract_io* new_IOLib(){return new Nano_io; }

void Nano_io::draw_full_rectangle(char color, const Rect& zone) const {}

void Nano_io::draw_rectangle(char color, const Rect& zone, int linewidth) const {}

void Nano_io::erase_surface(const Rect& zone) const {}

void Nano_io::print_text(const char* text, char color, int y, int z) const {
    std::cout << text << '\n';
}

void Nano_io::draw_boxed_text(const char* text, char color, char backgr_color, int y, int z, int width) const {
	std::cout << "[" << text << "]\n";
}

void Nano_io::message(const char* text) const {
	std::cout << "[Computer]: " << text << "\n";
}

void Nano_io::draw_permanent(const Rect& zone, char color, bool tapped, bool highlight, bool basicImg) const {
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

void Nano_io::disp_header(const Rect& zone, const char* name, int life, char state, bool highlight, Mana pool) const {
	if(life >= 1000) raise_error("Life total too high to be shown, most likely a bug");
	std::cout << "[]: Player " << name << "\n\t" << life << " life\t" << pool.m2t() << " mana\n";

	const char* phases_dcp[] = {"Begin", "First Main", "Declare Attackers", "Declare Blockers", "Damage", "Second Main", "End", "Not active"};
	std::cout << "\tCurrently: " << phases_dcp[state >> 5];
}

void Nano_io::disp_cardback(const Rect& zone, int oncard_number) const {
	std::cout << oncard_number << " cards\n";
}

void Nano_io::poster(const std::string name, Mana manacost, char color, const char* types,
	const std::vector<std::string> lines, int power, int toughness, char frametype, bool watermark) const {
	std::cout << "[Describing]: " << name << "\t" << manacost.m2t() << "\n";

	std::cout << "\t" << types << "\n";
	for(auto& i : lines){
		std::cout << i << "\n";
	}
	if(frametype == 1){
		char x[10]; sprintf(x, "(%d/%d)", power, toughness);
		std::cout << "\t\t" << x << "\n";
	}
	else if(frametype == 2){
		char x[5]; sprintf(x, "\\%d/", power);
		std::cout << "\t\t" << x << "\n";
	}
}

DirectioL Nano_io::get_direction_key(){
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

int Nano_io::getInt(int lowerBound, int upperBound){
	if(lowerBound >= upperBound) return lowerBound;
	int ret = lowerBound;
	while(1){
		std::cout << "[Input]: Please enter a number between " << lowerBound << " and " << upperBound << ": ";
		std::cin >> ret;
		if(ret >= lowerBound && ret <= upperBound) return ret;
		std::cout << "Sorry, try again\n";
	}
}

Nano_io::Nano_io(){}

void Nano_io::getResolution(int& y, int& z, bool& hasMouseSupport, int& linesize) const {
	y = 0;
	z = 0;
	hasMouseSupport = false;
	linesize = 0;
}

void Nano_io::harmonize(const Rect& message, const Rect& poster, int nb_winzones){}

void Nano_io::declare_window(int& leftY, int& topz, int width, int height){}

Nano_io::~Nano_io(){}