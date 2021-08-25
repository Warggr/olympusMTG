#include "12defaultUI.h"
#include "lib3_IO.h"
#include "yggdrasil/collection_tn.h"
#include "gameplay/permanents/4permanents.h"

bool DefaultUI::chooseattackers(CollectionTN<Creature>& cowards, StateTN<Creature>& warriors){
    myIO->message("Choose Attackers");
    bool ret = false;
    int i = 0;
    int yOff, zOff;
    Rect pos = permanentZones[4]->get_coordinates(&yOff, &zOff);
    for(auto iter = cowards.begin(); iter != cowards.end(); i++){
        if((iter->isUntapped() && iter->noSummonSick())){
            bool attacks = myIO->attack_switch(pos.y, pos.width, pos.z, 20);
            if(attacks){
                auto newatt = iter;
                myIO->disp(*newatt, pos, false); //disp creature normally
                iter++;
                newatt.get_pointed()->unstate(); //move creature to list "warriors"
                ret = true;
            }
            else{
                myIO->disp(*iter, pos, false); //disp creature normally
                iter++;
            }
        }
        else iter++;
        pos.shift(yOff, zOff);
    }
    return ret;
}

void DefaultUI::chooseblockers(CollectionTN<Creature>& defenders, StateTN<Creature>& attackers){
    int pos = 0;
    for(auto blocker = defenders.begin(); blocker != defenders.end(); blocker++){
        if(blocker->isUntapped()){ //untapped
            int y, z;
            permanentZones[4]->get_coordinates(pos, &y, &z);
            Creature* evilguy = blocker_switch(*blocker, y, z, attackers);
            if(evilguy){
                blocker->set_blocking();
                evilguy->add_blocker(&(*blocker));
            }
        }
        pos++;
    }
}

Creature* DefaultUI::blocker_switch(const Creature& blocker, int blockerY, int blockerZ, StateTN<Creature>& attackers) const {
    int pos_evilguy = 0;
    myIO->disp(blocker, Rect(blockerY, blockerZ, permanentYSize, permanentZSize), true);
    auto evilguy = attackers.end();
    while(true){ //getting creature to block
        DirectioL dir = myIO->get_direction_key();
        if(dir == BACK || dir == ENTER){
            if(evilguy != attackers.end()) return &(*evilguy);
            else return nullptr;
        }
        else if(evilguy != attackers.end()){
            int y, z;
            permanentZones[6]->get_coordinates(pos_evilguy, &y, &z);
            myIO->disp(*evilguy, Rect(y, z, permanentYSize, permanentZSize), false); //displaying villain normally
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
            myIO->disp(blocker, Rect(blockerY, blockerZ, permanentYSize, permanentZSize), false); //displaying blocker normally
        }
        if(evilguy != attackers.end()){
            int y, z;
            permanentZones[6]->get_coordinates(pos_evilguy, &y, &z);
            myIO->disp(*evilguy, Rect(y, z, permanentYSize, permanentZSize), true); //displaying villain
        }
        else{
            myIO->disp(blocker, Rect(blockerY, blockerZ, permanentYSize, permanentZSize), true);
        }
    }
}