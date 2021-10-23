#include "lib3_cliUI.h"
#include <list>
#include <iostream>
#include "headQ_rect.h"
#include "oracles/classes/card_oracle.h"
#include "control/3player.h"
#include "lib3_nanoIO.h"


using namespace std;

void NanoIO::message(const char* text) const {
	cout << "[Computer]: " << text << "\n";
}

void NanoIO::draw_permanent(const string& name, char, bool tapped, bool highlight) const {
    (void) highlight;
	cout << "\t<" << name << "> ";
	if(tapped) cout << "[TAPPED]";
	cout << "\n";
}

void NanoIO::disp_player(const Player& pl, int flags) const {
    (void) flags; //TODO
	cout << "[]: Player " << pl.getName() << "\n\t" << pl.getLife() << " life\t" << pl.manapool.m2t() << " mana\n";

	const char* phases_dcp[] = {"Begin", "First Main", "Declare Attackers", "Declare Blockers", "Damage", "Second Main", "End", "Not active"};
	cout << "\tCurrently: " << phases_dcp[pl.getPhase()] << "\n";
}

void NanoIO::disp_cardback(int oncard_number) const {
	cout << "\t" << oncard_number << " cards\n";
}

void print_getkey_help(){
	cout << "[HELP]: Possible commands:\nh\t\thelp\n";
	cout << "       Olympus-default commands:\n";
	cout << "         w    go up       s   go down\n";
	cout << "         a    go left     d   go right\n";
	cout << "         x/e  choose this option\n";
	cout << "         q/' 'go back\n";
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

NanoIO::NanoIO() { cout << "-----IO Library set to Nano. Welcome!-----\n"; }

NanoIO::~NanoIO(){ cout << "-----Ending Olympus Nano mode-----\n"; }

void NanoIO::setMenuScene() { cout << "---Menu---\n"; }

void NanoIO::setGameScene() { cout << "---Game Scene---\n"; }

std::string NanoIO::getTextInput(const char *question, bool newline) {
    std::cout << question;
    if(newline) std::cout << "\n>\t";
    std::string ret;
    std::getline( std::cin, ret );
    return ret;
}

void NanoIO::message(const std::string &text) const { cout << text; }

bool NanoIO::simpleChoice(const char* optTrue, const char* optFalse) {
    cout << optTrue << " (1/Y)";
    if(optFalse) cout << "\t" << optFalse << " (2/n)";
    cout << "\n";
    while(true) {
        char a; std::cin >> a;
        switch(a) {
            case '1': case 'Y': case 'y':
                return true;
            case '2': case 'N': case 'n':
                return false;
            default:
                std::cout << "Please enter Y or N (or 1/2)\n";
        }
    }
}

void NanoIO::disp(const CardOracle& oracle, int flags) const {
    cout << "[Describing]: " << oracle.getName() << "\t" << oracle.getCost().m2t();
    if(!(flags & INLINE)) cout << "\n";

    cout << "\t" << oracle.getType().toString() << "\n";

    if(!(flags & INLINE)) {
        int power, toughness, frametype;
        auto lines = oracle.allText(power, toughness, frametype);
        for (auto& i : lines) {
            cout << i << "\n";
        }
        if (frametype == 1) {
            char x[10];
            sprintf(x, "(%d/%d)", power, toughness);
            cout << "\t\t" << x << "\n";
        } else if (frametype == 2) {
            char x[5];
            sprintf(x, "\\%d/", power);
            cout << "\t\t" << x << "\n";
        }
    }
}

void NanoIO::disp_inrow(const Displayable* card, int number, int total, int flags) const {
    cout << '(' << number << '/' << total << ')' << ' '
        << (flags & HIGHLIGHT ? '[' : ' ') << card->describe() << (flags & HIGHLIGHT ? ']' : ' ') << '\n';
}

void NanoIO::disp(const Option& option, int flags) const {
    (void) option; (void) flags;
    cout << "[ERROR NOT IMPLEMENTED Displaying an option]\n";
}

void NanoIO::disp(const Card& card, int flags) const {
    disp(*card.oracle, flags);
}

BasicIO::checklistCallbackAction NanoIO::getNextPosition(abstract_iterator_wrapper* iter, uint& position, uint max) {
    std::cout << "What do you want to do? Enter the number which you want to select/unselect or -1 to submit\n";
    int what = getInt(-1, max);
    if (what == -1) {
        return BasicIO::commit;
    } else {
        uint where = static_cast<uint>(what);
        for (; position <= where; ++position, ++iter);
        for (; position >= where; --position, --iter);
        return BasicIO::change;
    }
}
