#include "3player.h"
#include "7game.h"

bool Player::chooseAttackers(){
	if(myboard.mycreas.empty()) return false;
	myboard.myattackers = myboard.mycreas.defNewState();
	bool ret = agent.chooseAttackers(myboard.mycreas);
	return ret;
}

void Player::chooseBlockers(StateTN<Creature>& attackers){
	agent.chooseBlockers(myboard.mycreas, attackers);
}

void Creature::splitDamage(Agent& agent) {
    agent.splitDamage(get_power(), assigned_bl);
}

void Player::damagestep(){
	phase = afterdamage;
	if(myboard.myattackers == nullptr) return;
	for(auto & attacker : *myboard.myattackers) {
	    attacker.splitDamage(getAgent());
	}
	for(auto & attacker : *myboard.myattackers){
		attacker.resolve_attack(nextopponent);
	}
	myboard.myattackers = nullptr;
	myboard.mycreas.restate(); //destroying attackers list
	Game::god->stateBasedActions();
}
