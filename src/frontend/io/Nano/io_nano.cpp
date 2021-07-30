#include "lib3_nanoIO.h"
#include <list>
#include <iostream>
#include "headQ_rect.h"

using namespace std;

AbstractIO* newIOLib(){return new NanoIO; }

void NanoIO::draw_full_rectangle(char color, const Rect& zone) const {}

void NanoIO::draw_rectangle(char color, const Rect& zone, int linewidth) const {}

void NanoIO::erase_surface(const Rect& zone) const {}

void NanoIO::print_text(const char* text, char color, int y, int z) const {
    cout << text << '\n';
}

void NanoIO::draw_boxed_text(const char* text, char color, char backgr_color, int y, int z, int width) const {
	cout << "[" << text << "]\n";
}

void NanoIO::message(const char* text) const {
	cout << "[Computer]: " << text << "\n";
}

void NanoIO::draw_permanent(const Rect& zone, const string& name, char color, bool tapped, bool highlight, bool basicImg) const {
	//if(highlight) is_iterating = true;
	cout << "\t<" << name << "> ";
	if(tapped) cout << "[TAPPED]";
	cout << "\n";
}

void NanoIO::disp_header(const Rect& zone, const char* name, int life, char state, bool highlight, Mana pool) const {
	//if(highlight) is_iterating = true;
	if(life >= 1000) gdebug(DBG_IOUI) << "Life total too high to be shown, most likely a bug";
	cout << "[]: Player " << name << "\n\t" << life << " life\t" << pool.m2t() << " mana\n";

	const char* phases_dcp[] = {"Begin", "First Main", "Declare Attackers", "Declare Blockers", "Damage", "Second Main", "End", "Not active"};
	cout << "\tCurrently: " << phases_dcp[state >> 5] << "\n";
}

void NanoIO::disp_cardback(const Rect& zone, int oncard_number) const {
	cout << "\t" << oncard_number << " cards\n";
}

void NanoIO::poster(const string& name, Mana manacost, char color, const char* types,
	const vector<string>& lines, int power, int toughness, char frametype, bool watermark) const {
	cout << "[Describing]: " << name << "\t" << manacost.m2t() << "\n";

	cout << "\t" << types << "\n";
	for(auto& i : lines){
		cout << i << "\n";
	}
	if(frametype == 1){
		char x[10]; sprintf(x, "(%d/%d)", power, toughness);
		cout << "\t\t" << x << "\n";
	}
	else if(frametype == 2){
		char x[5]; sprintf(x, "\\%d/", power);
		cout << "\t\t" << x << "\n";
	}
}

void print_getkey_help(){
	cout << "[HELP]: Possible commands:\nh\t\thelp\n";
	cout << "       Olympus-default commands:\n";
	cout << "         w    go up       s   go down\n";
	cout << "         a    go left     d   go right\n";
	cout << "         x/e  choose this option\n";
	cout << "         q/' 'go back\n";
}

DirectioL NanoIO::get_direction_key(){
	char c;
	while(true){
		cout << "[Input] What do you want to do? (h for possible commands): ";
		cin >> c;
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
			case 'k': throw UIClosedException();
			default: cout << "Unrecognized. Try 'h'.\n";
		}
	}
}

int NanoIO::getInt(int lowerBound, int upperBound){
	if(lowerBound >= upperBound) return lowerBound;
	int ret = lowerBound;
	while(true){
		cout << "[Input]: Please enter a number between " << lowerBound << " and " << upperBound << ": ";
		cin >> ret;
		if(ret >= lowerBound && ret <= upperBound) return ret;
		cout << "Sorry, try again\n";
	}
}

NanoIO::NanoIO(): is_iterating(false) { cout << "-----IO Library set to Nano. Welcome!-----\n"; }

void NanoIO::getResolution(int& y, int& z, bool& hasMouseSupport, int& linesize) const {
	y = 0;
	z = 0;
	hasMouseSupport = false;
	linesize = 0;
}

void NanoIO::harmonize(const Rect& message, const Rect& poster, int nb_winzones){}

void NanoIO::declare_window(int& leftY, int& topz, int width, int height){}

NanoIO::~NanoIO(){ cout << "-----Ending Olympus Nano mode-----\n"; }

void NanoIO::setMenuScene() { cout << "---Menu---\n"; }

void NanoIO::setGameScene() { cout << "---Game Scene---\n"; }

std::string NanoIO::getTextInput(const char *question) {
    std::cout << question << "\n>\t";
    std::string ret;
    std::cin >> ret;
    return ret;
}

void NanoIO::fulldisp() const { /*TODO*/ }

void NanoIO::print_text(const std::string &text, char color, int x, int y) const { cout << text << "\n"; }

void NanoIO::draw_boxed_text(const std::string &text, char color, char backgr_color, int left, int top,
                             int width) const {
    cout << "[" << text << "]\n";
}

void NanoIO::message(const std::string &text) const { cout << text; }

float NanoIO::gmouseY() { return 0; }

float NanoIO::gmouseZ() { return 0; }

bool NanoIO::gmouseActive() { return false; }

