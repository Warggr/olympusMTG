#include "lib3_cliUI.h"

using namespace std;

bool NanoIO::attackSwitch(int, int, int, int) const {
	cout << "[Input]: Do you want to attack with this creature? (Y/n): ";
	char rep;
	while(true){
		cin >> rep;
		switch(rep){
			case 'Y':
			case 'y':
				return true;
			case 'N':
			case 'n':
				return false;
			default: cout << "\tPlease enter Y or N.\n";
		}
	}
}
