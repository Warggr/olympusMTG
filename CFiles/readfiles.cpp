#include <cstdlib>
#include <cstdio>
#include "../HFiles/classes.h"

void check_safepoint(FILE* myfile, char c, const char* message){
	char x = fgetc(myfile);
	if(x!= c){
		god.gdebug(DBG_READFILE) << "Error" << message << ": expected '"<< x << "', got '"<<x<<"' instead\n";
		god.minimalKill();
		printf("Error %s: expected '%c', got '%c' instead\n", message, c, x);
		exit(1);
	}
}

externVarContainer::externVarContainer(){
	debug_log.open("debug_info.txt", std::ifstream::trunc);
	verbose_debug.open("debug_verbose.txt", std::ifstream::trunc);
}

struct externVarContainer god;

Game::Game(const char* filename, char debug_flags): stack(0), wanted_debug(debug_flags), haswon(false){
	god.game = this;
	FILE *myfile;
	myfile = fopen(filename, "r");
	if(!myfile){
		printf("File read error!\n");
		exit(1);
	}
	for(int i=0; i<2; i++){
		players[i] = new Player(myfile, this, (bool)i);
	}
	Player* oppptr = players[1];
	for(int i=0; i<2; i++){
		players[i]->set_opp(oppptr);
		oppptr = players[i];
	}
	active_player = players[0];
	fclose(myfile);
	for(int i=0; i<LOGLEN; i++){
		logbook[i] = 0;
	}
	initgraph();
}

Player::Player(FILE* myfile, Game* gm, char isup): Damageable(20), state(0xe0), metagame(gm), fperm{0}{
	shown_depth = (HEADER_X + 7*CARD_X)*isup;
	shown_direction = 1 - 2*isup;
	fgetc(myfile); //<
	for(int i = 0; i<10; i++){
		name[i] = fgetc(myfile);
		if(name[i] == '>'){
			name[i] = 0;
			break;
		}
	}
	fgetc(myfile); // newline
	myzones[0].init_name("library");
	myzones[1].init_name("Graveyard");
	myzones[2].init_name("exile");
	myzones[0].init(myfile);
	myzones[0].shuffle();
	myoptions[0] = NULL;
	for(int i=1; i<NBMYOPTS; i++) myoptions[i] = 0;
	possiblepool = 0;
	manapool = 0;
	draw(7);
}

void CardZone::init(FILE* myfile){
	first = 0; size = 0;
	while(1){
		int nb = 0;
		char a, cardname[30];
		while(1){ //getting number of cards
			a = fgetc(myfile);
			//printf("Waiting for number.. got %c\n", a);
			if(a == '/'){
				fgetc(myfile); //getting newline
				return;
			}
			if(a == ' ') break;
			nb = nb*10 + a - 0x30;
		}
		fgetc(myfile); //<
		for(int i=0; i<30; i++){ //getting name
			cardname[i] = fgetc(myfile);
			if(cardname[i] == '>'){
				cardname[i] = 0;
				break;
			}
		}
		fgetc(myfile); //space
		char tmp[20];
		int pos = 0;
		do{ //getting cost
			tmp[pos++] = fgetc(myfile);
		} while(tmp[pos-1] != ' ');
		int cost = t2m(tmp);
		fscanf(myfile, "%c", &a); //type
		fgetc(myfile); //space before '{'
		char type;
		switch(a){
			case 'I': type=0; break;
			case 'L': type=1; break;
			case 'A': type=2; break;
			case 'P': type=3; break;
			case 'C': type=4; break;
			case 'S': type=5; break;
			default: printf("Error while reading decks: %c is not a card type\n", a);
			exit(1);
		}
		char* flavor_text = 0; Ability* on_resolve = 0; char* parameter; int nb_parameters = 0; int nb_actabs = 0; PermOption* first_actab;
		read_rules_text(myfile, &on_resolve, &flavor_text, &parameter, &nb_parameters, &nb_actabs, &first_actab, type); //stops before newline
		for(char i=0; i<nb; i++){
			first = new Card(cardname, cost, type, on_resolve, (char) nb_parameters, parameter, nb_actabs, first_actab, flavor_text, first);
			size++;
		}
		god.gdebug(DBG_X_READFILE) << nb << "cards of type" << a << "named" << cardname <<"created\n";
	}
}