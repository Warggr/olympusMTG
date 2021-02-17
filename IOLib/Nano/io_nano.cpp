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

void Nano_io::draw_permanent(const Rect& zone, const std::string name, char color, bool tapped, bool highlight, bool basicImg) const {
	//if(highlight) is_iterating = true;
	std::cout << "\t<" << name << "> ";
	if(tapped) std::cout << "[TAPPED]";
	std::cout << "\n";
}

void Nano_io::disp_header(const Rect& zone, const char* name, int life, char state, bool highlight, Mana pool) const {
	//if(highlight) is_iterating = true;
	if(life >= 1000) raise_error("Life total too high to be shown, most likely a bug");
	std::cout << "[]: Player " << name << "\n\t" << life << " life\t" << pool.m2t() << " mana\n";

	const char* phases_dcp[] = {"Begin", "First Main", "Declare Attackers", "Declare Blockers", "Damage", "Second Main", "End", "Not active"};
	std::cout << "\tCurrently: " << phases_dcp[state >> 5] << "\n";
}

void Nano_io::disp_cardback(const Rect& zone, int oncard_number) const {
	std::cout << "\t" << oncard_number << " cards\n";
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

void print_getkey_help(){
	std::cout << "[HELP]: Possible commands:\nh\t\thelp\n";
	std::cout << "       Olympus-default commands:\n";
	std::cout << "         w    go up       s   go down\n";
	std::cout << "         a    go left     d   go right\n";
	std::cout << "         x/e  choose this option\n";
	std::cout << "         q/' 'go back\n";
}

DirectioL Nano_io::get_direction_key(){
	char c;
	while(1){
		std::cout << "[Input] What do you want to do? (h for possible commands): ";
		std::cin >> c;
		switch(c){
			case 'h': print_getkey_help(); break;
			case 'a': return LEFT;
			case 'd': return RIGHT;
			case 'w': return UP;
			case 's': return DOWN;
			case ' ':
			case 'q': return BACK;
			case 'x':
			case 'e': return ENTER;
			case 'k': god.call_ragnarok(); exit(1);
			default: std::cout << "Unrecognized. Try 'h'.\n";
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

Nano_io::Nano_io(){
	std::cout << "-----IO Library set to Nano. Welcome!-----\n";
}

void Nano_io::getResolution(int& y, int& z, bool& hasMouseSupport, int& linesize) const {
	y = 0;
	z = 0;
	hasMouseSupport = false;
	linesize = 0;
}

void Nano_io::harmonize(const Rect& message, const Rect& poster, int nb_winzones){}

void Nano_io::declare_window(int& leftY, int& topz, int width, int height){}

Nano_io::~Nano_io(){
	std::cout << "-----Ending Olympus Nano mode-----\n";
}