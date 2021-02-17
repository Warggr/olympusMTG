#include "lib3_nanoIO.h"

bool Nano_io::attack_switch(int leftY, int Ywidth, int topZ, int arrowlength) const {
	bool attacks = false;
	std::cout << "[Input]: Do you want to attack with this creature? (Y/n): ";
	char rep;
	while(1){
		std::cin >> rep;
		switch(rep){
			case 'Y':
			case 'y':
				return true;
			case 'N':
			case 'n':
				return false;
			default: std::cout << "\tPlease enter Y or N.\n";
		}
	}
}