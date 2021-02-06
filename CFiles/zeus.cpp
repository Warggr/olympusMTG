#include ".header_link.h"

struct externVarContainer god;

externVarContainer::externVarContainer(): game(0), wanted_debug(0), myUI(0), myIO(0){}

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

void externVarContainer::call_ragnarok() {
    delete myIO;
    delete myUI;
    debug_log.close();
    verbose_debug.close();
}

std::ofstream& externVarContainer::gdebug(char password){
	if((password & wanted_debug) != 0) return debug_log;
	else return verbose_debug;
}

Game::Game(const char* deck_1, const char* deck_2, char debug_flags): stack(0), haswon(false){
	god.initialize(this, debug_flags);
	stack_ui = god.myUI->declare_element(Abstract_ui::ELTYPE_STACK, 0);
	logbook_ui = god.myUI->declare_element(Abstract_ui::ELTYPE_LOGBOOK, 0);

	players[0] = new Player(deck_1, 1);
	players[1] = new Player(deck_2, 2);
	Player* oppptr = players[1];
	for(int i=0; i<2; i++){
		players[i]->set_opp(oppptr);
		oppptr = players[i];
	}
	active_player = players[0];
}

Game::~Game(){
    god.gdebug(DBG_RAGNAROK) << "Destroying the game...\n";
    for(int i=0; i<2; i++){
        if(players[i]) delete players[i];
    }
}