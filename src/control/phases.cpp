#include "3player.h"
#include "7game.h"

#include <algorithm>
//GUIDELINE: anything that calls statebasedactions() should be able to return afterwards
//anything that calls a function that calls statebasedactions should be able to exit afterwards

struct Phase{ //AFAIK there are no "additional step" cards, but there might be some day. In the meantime, a fixed-length list of steps will do.
    const int _nb_steps;
    void (Player::**_steps)(); //a list of pointers to methods, aka a pointer to a pointer to a function returning void

    constexpr Phase(int nb_steps, void (Player::**steps)()): _nb_steps(nb_steps), _steps(steps) {};
    bool operator()(Player* pl) const {
        for(int i = 0; i < _nb_steps; i++){
            (*pl.*(_steps[i]))();
            pl->emptyPool();
            if(Game::the_game->hasWon()) return true;
        }
        return false;
    }
};

bool Player::turn(){
    std::cout << "Starting " << getName() << "'s turn\n";
    std::forward_list<const Phase*> thisTurnsOrder = defaultPhaseOrder;
    for(auto & iter : thisTurnsOrder){
        if((*iter)(this)) return true;
    }
    return false;
}

void Player::untapstep(){
    phase = upkeep; nb_lands_remaining = 1;
    nb_mainphase = 0;
    std::for_each(myboard.pbegin(), myboard.pend(), [](Permanent& p){p.declareBeginturn(); });
}

void Player::upkeepstep(){}
void Player::drawstep(){
    draw(1);
}
void Player::mainphasemainstep(){
    nb_mainphase++;
    phase = main;
    std::cout << " Starting main phase" << '\n';
    choicephase(true);
}
void Player::declareattackersstep(){
    myboard.mycreas.unfold(&myboard.myattackers);
    if(!myboard.mycreas.empty())
        agent.chooseAttackers(myboard.myattackers);
    phase = afterattack;
}
void Player::declareblockersstep(){
    if(!myboard.myattackers.empty()) nextopponent->chooseBlockers(myboard.myattackers);
    phase = afterblock;
}
void Player::endstep(){
    phase = end;
}
void Player::cleanupstep(){
    std::for_each(myboard.mycreas.begin(), myboard.mycreas.end(), [](Creature& c){c.setLife(0); });
    phase = nonactive;
}

void Game::stateBasedActions(){
    for(auto& player : players){
        haswon = haswon | player.stateBasedActions();
    }
}

void Game::play() {
    while(true){
        for( Player& player : players ) {
            if(player.turn()) return;
        }
    }
}

bool Player::stateBasedActions(){
    if(life <= 0 or milledout) return true;
    for(auto iter = myboard.mycreas.begin(); iter != myboard.mycreas.end(); iter = iter){
        if(iter->getToughness() + iter->getLife() <= 0){
            auto i2 = iter;
            ++iter;
            i2->destroy();
        }
        else ++iter;
    }
    return false;
}

void (Player::*bSteps[])() = {&Player::untapstep, &Player::upkeepstep, &Player::drawstep};
constexpr Phase beginning_phase(3, bSteps);
void (Player::*mSteps[])() = {&Player::mainphasemainstep};
constexpr Phase main_phase(1, mSteps);
void (Player::*cSteps[])() = {&Player::declareattackersstep, &Player::declareblockersstep, &Player::damagestep};
constexpr Phase combat_phase(3, cSteps);
void (Player::*eSteps[])() = {&Player::endstep, &Player::cleanupstep};
constexpr Phase ending_phase(2, eSteps);

#define PHASEORDERTYPE std::forward_list<const Phase*>
const PHASEORDERTYPE Player::defaultPhaseOrder = PHASEORDERTYPE(
        {&beginning_phase, &main_phase, &combat_phase, &main_phase, &ending_phase}
        );
#undef PHASEORDERTYPE

Player::Player(Agent& agent, Deck&& deck): Gamer(agent.getName()), agent(agent), deck(std::move(deck)), myOptions(this) {
    agent.getViewer().registerMe(this);
    myLibrary.init(this->deck);
    myLibrary.shuffle();

    drawStartingHand();
}

constexpr int NB_STARTING_CARDS = 7;

void Player::drawStartingHand() {
    for(int i=0; i<NB_STARTING_CARDS + 1; i++) {
        if(agent.keepsHand(myLibrary.getCards())) {
            draw(NB_STARTING_CARDS);
            auto cardsDiscarded = agent.chooseCardsToKeep(myHand, i);
            for(auto& card: cardsDiscarded) {
                myLibrary.placeOnBottom(move_cardptr(card.unwrap()));
            }
            return;
        } else {
            myLibrary.shuffle();
        }
    }
}
