#include <iostream>

[[noreturn]] void raise_error(const std::string& message){
    god.gdebug(DBG_IMPORTANT) << "Error: " << message << std::endl;
    std::cout << "Error: " << message << std::endl;
    god.call_ragnarok();
    exit(1);
}

void externVarContainer::initialize(Game* gm, char debug_flags){
	game = gm;
	wanted_debug = debug_flags;
	debug_log.open("debug_info.txt", std::ifstream::trunc);
	verbose_debug.open("debug_verbose.txt", std::ifstream::trunc);
	myIO = new_IOLib();
	myUI = new_UILib(god.myIO);
}

std::ofstream& externVarContainer::gdebug(char password){
	if((password & wanted_debug) != 0) return debug_log;
	else return verbose_debug;
}
