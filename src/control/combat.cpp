#include "3player.h"
#include "7game.h"

bool Player::chooseAttackers(){
	if(myboard.mycreas.empty()) return false;
    myboard.mycreas.unfold(&myboard.myattackers);
	bool ret = agent.chooseAttackers(myboard.mycreas);
	return ret;
}

void Player::chooseBlockers(StateTN<Creature>& attackers){
	agent.chooseBlockers(myboard.mycreas, attackers);
}

void Creature::splitDamage(Agent& agent) {
    agent.splitDamage(getPower(), assigned_bl);
}

void Player::damagestep(){
	phase = afterdamage;
	if(myboard.myattackers.empty()) return;
	for(auto & attacker : myboard.myattackers) {
	    attacker.splitDamage(getAgent());
	}
	for(auto & attacker : myboard.myattackers){
		attacker.resolveAttack(nextopponent);
	}
	myboard.myattackers.restate(); //destroying attackers list
	Game::god->stateBasedActions();
}
