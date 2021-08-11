#include "control/3player.h"
#include "control/OutputManager.h"
#include "4permanents.h"
#include "../1general.h"
#include "../2cards.h"
#include "oracles/classes/2cards.h"
#include "oracles/classes/PermOption.h"
#include "headC_constants.h"

void Player::resolve_playland(uptr<Card> source){
	OutputManager::addToLog("  Land played");
	nb_lands_remaining -= 1;
	insert_permanent(std::move(source));
}

void Player::insert_permanent(uptr<Card> source){
	myboard.insert(std::move(source), this);
}

Land::Land(std::unique_ptr<Card> src, Player* pl): Permanent(std::move(src), pl){
	/*pl->possiblepool += src->get_cost();
	pl->highestpool += src->get_cost();
	pl->check_too_expensive();*/
}

Permanent::Permanent(std::unique_ptr<Card> src, Player* pl): Target(src->get_name()),
	source(std::move(src)), ctrl(pl), first_actab(nullptr), existing_statics(nullptr),
	nb_actabs(0), etbBeforeThisTurn(0), untapped(1), keywords(0), color(source->get_color())
{
	t_type = target_type::permanent;
	src->get_permabs(&first_actab, &nb_actabs);
	src->get_triggers(trigger_types::PermETB, triggers_permanent[0]);
	src->get_triggers(trigger_types::PermLTB, triggers_permanent[1]);
	src->get_triggers(trigger_types::PermStateChange, triggers_permanent[2]);
	src->get_triggers(trigger_types::PermBecomes, triggers_permanent[3]);

	triggers_permanent[0].trigger(pl, this);
}

Creature::Creature(std::unique_ptr<Card> src, Player* pl):
        Permanent(std::move(src), pl), Damageable(0, src.get()){
	t_type = target_type::creature;

	const char* tmp = src->get_flavor_text();
	self_power = tmp[0];
	self_toughness = tmp[1];
	set_power = tmp[0];
	set_toughness = tmp[1];
	src->get_triggers(trigger_types::CreaAttacks, triggers_creature[0]);
}

Planeswalker::Planeswalker(std::unique_ptr<Card> src, Player* pl):
    Permanent(std::move(src), pl), Damageable((int) src->get_flavor_text()[0], src.get()) {
    t_type = target_type::planeswalker;
    thisturn = false;

}

Damageable::Damageable(int lif, Card* source): life(lif){
	source->get_triggers(trigger_types::DamageableIsDamaged, is_damaged);
}

/*void Player::remove_permanent(Permanent* perm, int nb_zone){
	switch(nb_zone){
	    case 1: remove_permanent_inlist<Land>(&myboard.mylands, dynamic_cast<Land*>(perm)); break;
		case 2: remove_permanent_inlist<Artifact>(&myboard.myartos, dynamic_cast<Artifact*>(perm)); break;
		case 3: remove_permanent_inlist<Planeswalker>(&myboard.mysuperfriends, dynamic_cast<Planeswalker*>(perm)); break;
		case 4: remove_permanent_inlist<Creature>(&myboard.mycreas, dynamic_cast<Creature*>(perm)); break;
#ifdef NDEBUG
		default:
		god.gdebug(DBG_IMPORTANT) << "Error: Removed non-permanent" << std::endl;
		god.call_ragnarok();
		std::cout << "Error: Removed non-permanent" << std::endl;
		exit(1);
#endif
	}
}

template <class PermType>
void Player::remove_permanent_inlist(PContainer<PermType>* perms, PermType* asup){ //The source has already been sent to myGraveyard. This function only utterly deletes the Permanent.
	for(auto iter = perms->begin(); iter!=perms->end(); iter++){
		if(&(*iter) == asup){
			iter.get_pointed()->obliterate();
			return;
		}
	}
}*/

void Permanent::exile(){
	ctrl->puttozone(source, exile_zn); //putting source in myExile
	ctrl->remove_permanent(this);
}

void Permanent::destroy(){
	ctrl->puttozone(source, graveyard_zn); //putting source in myGraveyard
	ctrl->remove_permanent(this);
}

/*bool Permanent::directactivate(){
	if(nb_actabs == 1 && first_actab[0].get_ismana()){ //is it a mana ability?
		if(first_actab[0].iscastable(ctrl)){ //if it's castable
			if(first_actab[0].get_tapsymbol()){ //tap it, if needed
				if((flags & 1)){
					flags = flags & (~1);
				}
				else return false; //already tapped->don't bother with this permanent
			}
			ctrl->manapool -= first_actab[0].cost; //pay the mana
			first_actab[0].straight_cast(ctrl); //add the mana!
			return false; //and pass on to sth else
		}
		else return false; //not castable? don't bother
	}
	else return true; //not a mana ability? Return the permanent, takeopts will know what to do
}*/

void Permanent::activate(){
	if(nb_actabs == 1){
		if(first_actab[0].get_tapsymbol()) untapped = 0;
		ctrl->manapool -= first_actab[0].cost;
		first_actab[0].cast_opt(ctrl, *this);
	}
}

Permanent::type Permanent::getType() const {
    return source->get_type().toPermType();
}

void Land::untap(){
	Permanent::untap();
	//ctrl->possiblepool += source->get_cost();
}

void Planeswalker::activate() {
    /*thisturn = true;
    god.myIO->message("Press UP, SPACE or DOWN to select a loyalty ability");
    char getkey = god.myIO->get_direction_key();
    char x;
    switch (getkey) {
        case BACK: x = 2; break;
        case DOWN: x = 1; break;
        case UP:
        default: x = 0; break;
    }
    loyalty -= loyalty_costs[(int) x];
    addtostack(new Resolvable(ctrl, loyalty_abs + x, (Target*) this));*/
}
