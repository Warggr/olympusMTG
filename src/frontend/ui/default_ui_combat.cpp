#include "12defaultUI.h"

bool DefaultUI::chooseattackers(PContainer<Creature>& cowards, PContainer<Creature>& warriors, char player_id){
    myIO->message("Choose Attackers");
    bool ret = false;
    int i = 0;
    int yOff, zOff;
    Rect pos = permanentZones[(player_id-1) * 5 + 3]->get_coordinates(&yOff, &zOff);
    for(auto iter = cowards.begin(); iter != cowards.end(); i++){
        if((iter->get_flags()&3) == 3){ //untapped and no summoning sickness
            int zPos = (player_id == 1) ? pos.z + pos.height : pos.z; //getting either top or bottom Z
            bool attacks = myIO->attack_switch(pos.y, pos.width, zPos, (player_id == 1) ? 20 : -20);
            if(attacks){
                auto newatt = iter;
                newatt->disp(pos, false); //disp creature normally
                newatt->reset_flags(1); //tap creature
                iter++;
                newatt.get_pointed()->unstate(); //move creature to list "warriors"
                ret = true;
            }
            else{
                iter->disp(pos, false); //disp creature normally
                iter++;
            }
        }
        else iter++;
        pos.shift(yOff, zOff);
    }
    return ret;
}

void DefaultUI::chooseblockers(PContainer<Creature>& defenders, PContainer<Creature>& attackers, UIElement* defenderDisplay, UIElement* attackerDisplay){
    int pos = 0;
    for(auto blocker = defenders.begin(); blocker != defenders.end(); blocker++){
        if(blocker->get_flags()&1){ //untapped
            int y, z;
            defenderDisplay->get_coordinates(pos, &y, &z);
            Creature* evilguy = blocker_switch(*blocker, y, z, attackers, attackerDisplay);
            if(evilguy){
                blocker->set_blocking();
                evilguy->add_blocker(&(*blocker));
            }
        }
        pos++;
    }
}

Creature* DefaultUI::blocker_switch(const Creature& blocker, int blockerY, int blockerZ, PContainer<Creature>& attackers, UIElement* attackerIO) const {
    int pos_evilguy = 0;
    blocker.disp(Rect(blockerY, blockerZ, permanentYSize, permanentZSize), true);
    auto evilguy = attackers.end();
    while(1){ //getting creature to block
        DirectioL dir = myIO->get_direction_key();
        if(dir == BACK || dir == ENTER){
            if(evilguy != attackers.end()) return &(*evilguy);
            else return 0;
        }
        else if(evilguy != attackers.end()){
            int y, z;
            attackerIO->get_coordinates(pos_evilguy, &y, &z);
            evilguy->disp(Rect(y, z, permanentYSize, permanentZSize), false); //displaying villain normally
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
            blocker.disp(Rect(blockerY, blockerZ, permanentYSize, permanentZSize), false); //displaying blocker normally
        }
        if(evilguy != attackers.end()){
            int y, z;
            attackerIO->get_coordinates(pos_evilguy, &y, &z);
            evilguy->disp(Rect(y, z, permanentYSize, permanentZSize), true); //displaying villain
        }
        else{
            blocker.disp(Rect(blockerY, blockerZ, permanentYSize, permanentZSize), true);
        }
    }
}