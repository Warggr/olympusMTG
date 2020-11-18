#include "../HFiles/classes.h"
#include <ncurses.h>

//GUIDELINE: anything that calls statebasedactions() should be able to return afterwards
//anything that calls a function that calls statebasedactions should be able to exit afterwards

bool Player::turn(){
	metagame->addtolog("Starting your turn");
	untapstep(); if(metagame->haswon) return true;
	upkeepstep(); if(metagame->haswon) return true;
	drawstep(); if(metagame->haswon) return true;
	mainphase(); if(metagame->haswon) return true;
	declareattackersstep(); if(metagame->haswon) return true;
	declareblockersstep(); if(metagame->haswon) return true;
	damagestep(); if(metagame->haswon) return true;
	mainphase(); if(metagame->haswon) return true;
	endstep(); if(metagame->haswon) return true;
	return false;
}

void Player::untapstep(){
	for(int i=0; i<4; i++){ //UNTAPPING EVERYTHING
		if(fperm[i]) fperm[i]->set_board_flags(3); //untapped and no invocation sickness
	}
	metagame->disp();
}
void Player::upkeepstep(){
	begin_phase();
	state = state&15; //resetting time and land flags
	end_phase();
}
void Player::drawstep(){
	draw(1);
	end_phase();
}
void Player::mainphase(){
	begin_phase();
	metagame->addtolog(" Starting main phase");
	choicephase(true);
	end_phase();
}
void Player::declareattackersstep(){
	begin_phase();
	chooseattackers();
	end_phase();
}
void Player::declareblockersstep(){
	begin_phase();
	if(fperm[4]) nextopponent->chooseblockers();
	end_phase();
}
void Player::endstep(){
	begin_phase();
	end_phase();
	state += 32;
}

void Player::empty_pool(){
	possiblepool = minus(possiblepool, manapool); //saying goodbye to all mana currently in pool
	manapool = 0;
	disp_header();
}

void Player::end_phase(){
	empty_pool();
}

void Player::begin_phase(){
	state += 32;
	disp_header();
}

void Game::statebasedactions(){
	for(int i=0; i<2; i++){
		if(players[i]->get_life() <= 0) haswon = true;
	}
	for(int i=0; i<2; i++){
		for(Creature* iter = (Creature*) players[i]->fperm[3]; iter!=0; iter= (Creature*) iter->next){
			if(iter->get_toughness() + iter->get_life() <= 0){
				iter->destroy(false);
			}
		}
	}
}