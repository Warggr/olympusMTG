#include "12defaultUI.h"
#include "lib3_IO.h"
#include "yggdrasil/collection_tn.h"
#include "yggdrasil/state_n.h"
#include "gameplay/permanents/4permanents.h"

bool DefaultUI::chooseattackers(Y_Hashtable<Creature>& cowards){
    myIO->message("Choose Attackers");
    bool ret = false;
    int i = 0;
    int yOff, zOff;
    Rect pos = screen.board().players()[0].board().policy.attackers.getCoordinates(&yOff, &zOff);
    for(auto iter = cowards.begin(); iter != cowards.end(); i++){
        if((iter->isUntapped() && iter->noSummonSick())){
            bool attacks = myIO->attackSwitch(pos.y, pos.width, pos.z, 20);
            if(attacks){
                auto newatt = iter;
                myIO->disp(*newatt, pos, false); //disp creature normally
                ++iter;
                //newatt.getPointed()->unstate(); //move creature to list "warriors"
                ret = true;
            }
            else{
                myIO->disp(*iter, pos, false); //disp creature normally
                ++iter;
            }
        }
        else ++iter;
        pos.shift(yOff, zOff);
    }
    return ret;
}

void DefaultUI::chooseblockers(Y_Hashtable<Creature>& defenders, StateTN<Creature>& attackers){
    int pos = 0;
    for(auto blocker = defenders.begin(); blocker != defenders.end(); ++blocker){
        if(blocker->isUntapped()){ //untapped
            Creature* evilguy = blockerSwitch(*blocker, pos, attackers);
            if(evilguy){
                blocker->setBlocking();
                evilguy->addBlocker(&(*blocker));
            }
        }
        pos++;
    }
}

Creature* DefaultUI::blockerSwitch(const Creature& blocker, int blockerIndex, StateTN<Creature>& attackers) {
    int pos_evilguy = 0;
    myIO->disp(blocker, screen.board().players()[1].board().policy.attackers.getCoordinates(blockerIndex), true);
    auto evilguy = attackers.end();
    while(true){ //getting creature to block
        DirectioL dir = myIO->get_direction_key();
        if(dir == BACK || dir == ENTER){
            if(evilguy != attackers.end()) return &(*evilguy);
            else return nullptr;
        }
        else if(evilguy != attackers.end()){
            myIO->disp(*evilguy, screen.board().players()[0].board().policy.attackers.getCoordinates(pos_evilguy), false);
            //displaying villain normally
            switch(dir){
                case DOWN: evilguy = attackers.end(); break;
                case LEFT:
                    if(evilguy != attackers.begin()){
                        --pos_evilguy;
                        --evilguy;
                    } break;
                case RIGHT:
                    ++evilguy;
                    if(evilguy == attackers.end()) --evilguy;
                    else pos_evilguy++;
                    break;
                default: break;
            }
        }
        else if(dir == UP){
            evilguy = attackers.begin();
            pos_evilguy = 0;
            myIO->disp(blocker, screen.board().players()[1].board().policy.attackers.getCoordinates(blockerIndex), false);
            //displaying blocker normally
        }
        if(evilguy != attackers.end()){
            myIO->disp(*evilguy, screen.board().players()[0].board().policy.attackers.getCoordinates(pos_evilguy), true);
            //displaying villain
        }
        else{
            myIO->disp(blocker, screen.board().players()[1].board().policy.attackers.getCoordinates(blockerIndex), true);
        }
    }
}
