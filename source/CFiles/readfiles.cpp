#include "../HFiles/olympus_main.h"

void check_safepoint(std::ifstream& myfile, char c, const char* message){
	char x = myfile.get();
	if(x != c){
		raise_error(std::string(message) + ": expected '" + c + "', got '" + x + "' instead");
	}
}

void raise_error(std::string message){
	god.gdebug(DBG_READFILE | DBG_X_READFILE | DBG_IMPORTANT) << "Error: " << message << std::endl;
	god.minimalKill();
	std::cout << "Error: " << message << std::endl;
	exit(1);
}

externVarContainer::externVarContainer(): game(0), debug_log(), verbose_debug(), wanted_debug(0), myUI(0), myIO(0){
	debug_log.open("debug_info.txt", std::ifstream::trunc);
	verbose_debug.open("debug_verbose.txt", std::ifstream::trunc);
}

struct externVarContainer god;

Game::Game(const char* filename, char debug_flags): stack(0), haswon(false){
	god.game = this;
	god.myIO = new Allegro_io;
	god.myUI = new Default_ui(god.myIO);
	god.wanted_debug = debug_flags;
	stack_ui = god.myUI->declare_element(Abstract_ui::ELTYPE_STACK, 0);
	logbook_ui = god.myUI->declare_element(Abstract_ui::ELTYPE_LOGBOOK, 0);

	std::ifstream myfile(filename);
	if(!myfile){
		raise_error("File read error!");
	}
	for(int i=0; i<2; i++){
		players[i] = new Player(myfile, this, i+1);
	}
	Player* oppptr = players[1];
	for(int i=0; i<2; i++){
		players[i]->set_opp(oppptr);
		oppptr = players[i];
	}
	active_player = players[0];
	myfile.close();
	for(int i=0; i<LOGLEN; i++){
		logbook[i] = 0;
	}
}

Player::Player(std::ifstream& myfile, Game* gm, char id): Damageable(20), state(0xe0), player_id(id), permUI{0}, optZone(0), metagame(gm) {
	target_flags = 0x80;
	myfile.get(); //<
	myfile.get(name, 10, '>'); //get max. 10 characters, ending with '>'
	myfile.get(); // newline
	
	myzones[0].init_name("Library");
	myzones[1].init_name("Graveyard");
	myzones[2].init_name("Exile");
	myzones[0].init(myfile);
	myfile.get(); //getting '/' character

	myzones[0].shuffle();
	myoptions[0] = NULL;
	for(int i=1; i<NBMYOPTS; i++) myoptions[i] = 0;
	possiblepool = 0;
	manapool = 0;
	draw(7);

	optZone = god.myUI->get_optionzone();
	for(int i=0; i<5; i++){
		permUI[i] = god.myUI->declare_element(i + Abstract_ui::ELTYPE_PERMANENTS, player_id);
	}
}

void CardZone::init(std::ifstream& myfile){
	size = 0;
	while(1){
		int nb = 0;
		char cardname[30];
		if(myfile.peek() == '/'){
			myfile.get();
			return;
		}
		myfile >> std::skipws >> nb; //getting number of cards
		check_safepoint(myfile, ' ', "before card name");
		check_safepoint(myfile, '<', "before card name"); //<
		myfile.get(cardname, 30, '>'); //get max. 30 characters, ending with '>'
		check_safepoint(myfile, '>', "after card name"); //space

		char tmp[20];
		myfile >> tmp; //should end with ' '
		int cost = t2m(tmp);
		char typeOfCard;
		myfile >> typeOfCard;

		char type;
		switch(typeOfCard){
			case 'I': type=0; break;
			case 'L': type=1; break;
			case 'A': type=2; break;
			case 'P': type=3; break;
			case 'C': type=4; break;
			case 'S': type=5; break;
			default: raise_error(std::string("while reading decks: ") + typeOfCard + " is not a card type");
		}
		std::shared_ptr<CardOracle> cardrules = std::make_unique<CardOracle>(myfile, cardname, cost, type); //stops after newline
		for(char i=0; i<nb; i++){
			Card* first = new Card(cardrules);
			cards.push_front(first);
			size++;
		}
		god.gdebug(DBG_X_READFILE) << nb << "cards of type" << typeOfCard << "named" << cardname << "created\n";
	}
}