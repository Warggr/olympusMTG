#include "3player.h"

#include <algorithm>
//GUIDELINE: anything that calls statebasedactions() should be able to return afterwards
//anything that calls a function that calls statebasedactions should be able to exit afterwards

struct Phase{ //AFAIK there are no "additional step" cards, but there might be some day. In the meantime, a fixed-length list of steps will do.
	const int _nb_steps;
	void (Player::**_steps)(); //a list of pointers to methods, aka a pointer to a pointer to a function returning void

	Phase(int nb_steps, void (Player::**steps)()): _nb_steps(nb_steps), _steps(steps) {};
	bool operator()(Player* pl) const {
		for(int i = 0; i < _nb_steps; i++){
			pl->disp_header();
			(*pl.*(_steps[i]))();
			pl->empty_pool();
			if(Game::god->haswon) return true;
		}
		return false;
	}
};

Player::Player(const char* deck_name, char id): Target(&name), Damageable(20), name{0}, state(0xe0), player_id(id), permUI{0}, optZone(0){
    target_flags = 0x80;

    std::string cacheName = std::string("Materials/decks/.binary/deck") + (char)(id + '0');
    std::ifstream inputCache(cacheName, std::ios::binary | std::ios::in);
    int version = 0;
    if(inputCache.is_open()) inputCache.read((char*) &version, sizeof(int));
    if(version == OLYMPUS_BINARYCOMPAT_VERSION){
        std::cout << "Binary preformatted file found!" << std::endl;
        char x;
        check_canary('<', inputCache);
        inputCache.read(&x, sizeof(char));
        char name_tmp[(int) x];
        inputCache.read(name_tmp, x * sizeof(char));
        name = std::string(name_tmp);
        check_canary('-', inputCache);

        myzones[0].init(inputCache);
        inputCache.close();
    }
    else{
        std::cout << "Binary file not found.";
        std::ifstream myfile(std::string("Materials/decks/") + deck_name + ".mtgoly");
        if(!myfile.is_open()) raise_error("Deck name not found");

        std::ofstream outputCache(".tmp", std::ios::binary | std::ios::out);
        int i = OLYMPUS_BINARYCOMPAT_VERSION;
        outputCache.write((char*) &i, sizeof(int));
        set_canary('<', outputCache);

        std::cout << "Interpreting plain text deck..." << std::endl;
        myfile.get(); //<
        char tmp[11];
        myfile.get(tmp, 10, '>'); //get max. 10 characters, without ending '>'
        name = std::string(tmp);
        char x; for(x = 0; tmp[(int) x] != '\0'; x++);
        outputCache.write(&x, sizeof(char));
        outputCache.write(tmp, x * sizeof(char));
        set_canary('-', outputCache);

        myfile.get(); // newline
        myzones[0].init(myfile, outputCache);
        outputCache.close();
        rename(".tmp", cacheName.c_str());
        myfile.close();
    }

    myzones[0].init_name("Library");
    myzones[1].init_name("Graveyard");
    myzones[2].init_name("Exile");

    myzones[0].shuffle();
    myoptions[0] = nullptr;
    for(int i=1; i<NBMYOPTS; i++) myoptions[i] = 0;
    possiblepool = 0;
    manapool = 0;
    draw(7);

    optZone = god.myUI->get_optionzone();
    for(int i=0; i<5; i++){
        permUI[i] = god.myUI->declare_element(i + Abstract_ui::ELTYPE_PERMANENTS, player_id);
    }
}

void CardZone::init(std::ifstream& myfile, std::ofstream& binaryLog){
    size = 0;
    set_canary('o', binaryLog);
    while(myfile.peek() != EOF){ //EOF char not encountered
        char nb = 0;
        int nb_i;
        myfile >> std::skipws >> nb_i; nb = (char) nb_i; //getting number of cards

        binaryLog.write(&nb, sizeof(char));
        std::shared_ptr<CardOracle> cardrules = std::make_unique<CardOracle>(myfile, cardname, cost, type); //stops after newline
        cardrules->write_binary(binaryLog);
        for(char i=0; i<nb; i++){
            Card* first = new Card(cardrules);
            cards.push_front(first);
            size++;
        }
        god.gdebug(DBG_X_READFILE) << (int) nb << "cards of type " << typeOfCard << " named " << cardname << " created" << std::endl;
    }
    char x = 0; //! zero cards means 'end of file'
    binaryLog.write(&x, sizeof(char));
    set_canary('m', binaryLog);
}

bool Player::turn(){
	god.game->addtolog("Starting your turn");
	std::forward_list<const Phase*> thisTurnsOrder = defaultPhaseOrder;
	for(auto & iter : thisTurnsOrder){
		if((*iter)(this)) return true;
	}
	return false;
}

void Player::untapstep(){
	state = state & 15; //resetting time and land flags
	nb_mainphase = 0;
	std::for_each(myboard.pbegin(), myboard.pend(), [](Permanent& p){p.declare_beginturn(); });
	god.game->disp();
}

void Player::upkeepstep(){}
void Player::drawstep(){
	draw(1);
}
void Player::mainphasemainstep(){
	nb_mainphase++;
	SET_TIME_FLAGS(0x20);
	if(nb_mainphase != 0) state += 0x80;
	god.game->addtolog(" Starting main phase");
	choicephase(true);
}
void Player::declareattackersstep(){
	SET_TIME_FLAGS(0x40);
	chooseattackers();
}
void Player::declareblockersstep(){
	SET_TIME_FLAGS(0x60);
	if(myboard.myattackers) nextopponent->chooseblockers(*myboard.myattackers, permUI[4]);
}
void Player::endstep(){
	SET_TIME_FLAGS(0xc0);
}
void Player::cleanupstep(){
	std::for_each(myboard.mycreas.begin(), myboard.mycreas.end(), [](Creature& c){c.set_life(0); });
	SET_TIME_FLAGS(0xe0);
}
void Player::empty_pool(){
	possiblepool -= manapool; //saying goodbye to all mana currently in pool
	manapool = Mana();
}

void Game::statebasedactions(){
	for(int i=0; i<2; i++){
		haswon = haswon | players[i]->statebasedactions();
	}
}

bool Player::statebasedactions(){
	if(life <= 0) return true;
	for(auto iter = myboard.mycreas.begin(); iter != myboard.mycreas.end(); iter = iter){
		if(iter->get_toughness() + iter->get_life() <= 0){
			auto i2 = iter;
			iter++;
			i2->destroy();
		}
		else iter++;
	}
	return false;
}

void (Player::*bSteps[])() = {&Player::untapstep, &Player::upkeepstep, &Player::drawstep};
const Phase beginning_phase(3, bSteps);
void (Player::*mSteps[])() = {&Player::mainphasemainstep};
const Phase main_phase(1, mSteps);
void (Player::*cSteps[])() = {&Player::declareattackersstep, &Player::declareblockersstep, &Player::damagestep};
const Phase combat_phase(3, cSteps);
void (Player::*eSteps[])() = {&Player::endstep, &Player::cleanupstep};
const Phase ending_phase(2, eSteps);

const std::forward_list<const Phase*> Player::defaultPhaseOrder = std::forward_list<const Phase*>({&beginning_phase, &main_phase, &combat_phase, &main_phase, &ending_phase});