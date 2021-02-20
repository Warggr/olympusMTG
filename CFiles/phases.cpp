#include ".header_link.h"
#include "../Yggdrasil/headB_board.h"

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
			if(god.game->haswon) return true;
		}
		return false;
	}
};

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