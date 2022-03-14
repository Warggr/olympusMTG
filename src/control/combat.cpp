#include "3player.h"
#include "7game.h"

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
    myboard.myattackers.restate(); //destroying attackers list

    src::logger& lg = DBG_YGGDRASIL::get();
    logging::record rec = lg.open_record();
    logging::record_ostream strm(rec);

    myboard.disp(0, strm);
    strm.flush();
    lg.push_record(boost::move(rec));

    Game::god->stateBasedActions();
}
