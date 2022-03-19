#include "control/3player.h"
#include "4permanents.h"
#include "../1general.h"
#include "../2cards.h"
#include "classes/card_oracle.h"
#include "classes/perm_option.h"
#include "headE_enums.h"

void Player::resolvePlayland(card_ptr source){
    nb_lands_remaining -= 1;
    myboard.insert(move_cardptr(source), this);
}

Permanent::Permanent(card_ptr src, Player* pl): Target(src->getName()), source(move_cardptr(src)),
    ctrl(pl), first_actab(nullptr), nb_actabs(0),
    etbBeforeThisTurn(0), untapped(1), keywords(0), color(source->getColor())
{
    t_type = target_type::permanent;
    source->getPermabs(&first_actab, &nb_actabs);
    source->getTriggers(trig_type::etb, triggers_permanent[0]);
    source->getTriggers(trig_type::ltb, triggers_permanent[1]);
    source->getTriggers(trig_type::statechange, triggers_permanent[2]);
    source->getTriggers(trig_type::becomes, triggers_permanent[3]);

    triggers_permanent[0].trigger(pl, this);
}

Creature::Creature(card_ptr src, Player* pl):
        Permanent(move_cardptr(src), pl), Damageable(0, raw_cardptr(source)){
    t_type = target_type::creature;

    const char* tmp = source->getFlavorText();
    self_power = tmp[0];
    self_toughness = tmp[1];
    set_power = tmp[0];
    set_toughness = tmp[1];
    source->getTriggers(trig_type::is_damaged, triggers_creature[0]);
}

Planeswalker::Planeswalker(card_ptr src, Player* pl):
    Permanent(move_cardptr(src), pl), Damageable(static_cast<int>(source->getFlavorText()[0]), source) {
    t_type = target_type::planeswalker;
    thisturn = false;
}

Damageable::Damageable(int lif, const Card* source): life(lif){
    source->getTriggers(trig_type::is_damaged, is_damaged);
}

void Permanent::exile(){
    ctrl->putToZone(source, Player::exile); //putting source in myExile
    ctrl->myboard.remove(this);
}

void Permanent::destroy(){
    ctrl->putToZone(source, Player::graveyard); //putting source in myGraveyard
    ctrl->myboard.remove(this);
}

void Permanent::activate(){
    if(nb_actabs == 1){
        first_actab[0].castOpt(ctrl);
    }
}

void Planeswalker::activate() {
    //TODO
}

const Option* Permanent::chooseOptionAction() const {
    return options.at(ctrl->getAgent().chooseAmong(options));
}
