#include "3player.h"
#include "7game.h"
#include "logging.h"

void Player::chooseBlockers(StateTN<Creature>& attackers){
    agent.chooseBlockers(myboard.mycreas, attackers);
}

void Creature::splitDamage(Agent& agent) {
    agent.splitDamage(getPower(), assigned_bl);
}

void Player::damagestep(){
    phase = afterdamage;
    if(myboard.myattackers.empty()) {
        myboard.myattackers.restate(); //destroying attackers list
        return;
    }

    for(auto & attacker : myboard.myattackers) {
        attacker.splitDamage(getAgent());
    }
    for(auto & attacker : myboard.myattackers){
        attacker.resolveAttack(nextopponent);
    }

    OPEN_LOG_AS(DBG_YGGDRASIL, strm);
    myboard.disp(0, strm);
    CLOSE_LOG(strm);

    myboard.myattackers.restate(); //destroying attackers list

    Game::the_game->stateBasedActions();
}
