#include "classes/8options.h"
#include "3player.h"
#include "7game.h"
#include "gameplay/resolvables/stack.h"

//Player calls choicephase.
//Player::choicephase calls choose_and_use_opt. Calls check_and_pop. Popped option is returned and cast (aka added to stack, or land put in play)
//Function returns 0 if Default was cast.
//Player::choicephase lets opponent choose_and_use_opt afterwards.
//When both return 0, Player::choicephase resolves the first EmptyOption of the stack via Spell::resolve. Then new alternating choose_and_use_opt
//When both pass and the stack is empty, Player::choicephase returns.

void Player::choicephase(bool sorceryspeed){
    while(true){
        Player* lastActive = nullptr;
        if(chooseAndUseOpt(sorceryspeed)) lastActive = this;
        Player* currentprio = nextopponent;
        do{
            if(currentprio->chooseAndUseOpt(false)) lastActive = currentprio;
            currentprio = currentprio->nextopponent;
        } while(not (currentprio == lastActive || (lastActive == nullptr && currentprio == this)));
        //all player passed; resolving first spell
        auto toresolve = Stack::god->popFromStack();
        if(toresolve){
            gdebug(DBG_TARGETING) << "RESOLVING A SPELL\n";
            toresolve->resolve();
        }
        else return; //if there is no first spell, then the phase/step ends
    }
}

bool Player::chooseAndUseOpt(bool sorceryspeed){ //AKA "giving priority". Returns false if a pass option was chosen
    Game::the_game->stateBasedActions(); //"before any player receives priority, state-based actions are done"

    if(!myOptions.hasOptions(sorceryspeed)) return false;

    bool hasCastAnything = false;
    while(true){
        const Option::CastingContext context = { sorceryspeed, this };

        auto* choice = const_cast<Option*>(agent.chooseOpt(context)); //chooses opt, pops it and returns it, returns 0 if passing was chosen
        if(!choice) return hasCastAnything;
        if(choice->isCastable(context) and choice->castOpt(this)){ //casts the spell and deletes the option
            if(!Stack::god->stackIsEmpty()) sorceryspeed = false;
            hasCastAnything = true;
        } else {
            agent.getViewer().message("Can't cast this option");
        }
    }
}

bool Player::addTriggersToStack(){
    if(prestack.empty()) return false;
    //TODO: the player may order his triggers
    while(!(prestack.empty())){
        auto res = std::make_unique<Resolvable>(this, prestack.front().preRes, prestack.front().origin->getTarget());
        Stack::god->addToStack(std::move(res));
        prestack.pop_front();
    }
    return true;
}
