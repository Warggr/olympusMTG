#include ".header_link.h"
#include "../HFiles/8options.h"
#include "../HFiles/9modifs.h"
#include "../Yggdrasil/head6_iterators.h"

//Resolvables are deleted externally by takeopts after having been resolved.
//As a general rule, the one to take the Resolvable out of the stack is the one to destroy it
void Spell::resolve(){
	if(on_resolve) on_resolve->activate(list_of_targets, ctrl, this);
	switch(source->get_type()){
		case 0:
		case 5:
			ctrl->puttozone(source, 1); break;
		case 2:
		case 3:
		case 4:
			ctrl->insert_permanent(source);
	}
}

void Resolvable::resolve(){
	if(on_resolve) on_resolve->activate(list_of_targets, ctrl, origin->getTarget());
	//the 'if(on_resolve) is there only for two reasons: 1. programming rigorism and 2.Throes of Chaos'
	//really EVERY instant or sorcery will have an on_resolve (except of course Throes of Chaos)
}

Resolvable* PlayLand::cast_opt(Player* pl){
	pl->resolve_playland(source);
	delete this;
	return nullptr;
}

void Player::resolve_playland(Card* source){
	god.game->addtolog("  Land played");
	set_flags(16);
	insert_permanent(source);
}

void Player::insert_permanent(Card* source){
	myboard.insert(source, this);
}

Land::Land(Card* src, Player* pl): Permanent(src, pl, 0){
	pl->possiblepool += src->get_cost();
	pl->highestpool += src->get_cost();
	pl->check_too_expensive();
}

Permanent::Permanent(Card* src, Player* pl, int nb_zone): Target(src->get_name_ref()),
	source(src), ctrl(pl), first_actab(0), existing_statics(0),
	nb_actabs(0), flags(1), keywords(0), color(source->get_color())
{
	t_type = 0x40;
	src->get_permabs(&first_actab, &nb_actabs);
	src->get_triggers(olympus::trigger_types::PermETB, triggers_permanent[0]);
	src->get_triggers(olympus::trigger_types::PermLTB, triggers_permanent[1]);
	src->get_triggers(olympus::trigger_types::PermStateChange, triggers_permanent[2]);
	src->get_triggers(olympus::trigger_types::PermBecomes, triggers_permanent[3]);
	pl->disp_zone(3);
	triggers_permanent[0].trigger(pl, this);
}

Creature::Creature(Card* src, Player* pl): Permanent(src, pl, 3), Damageable(0, src){
	t_type = 0x50; //Permanent(0x40) and Creature (0x10)

	const char* tmp = src->get_flavor_text();
	self_power = tmp[0];
	self_toughness = tmp[1];
	set_power = tmp[0];
	set_toughness = tmp[1];
	src->get_triggers(olympus::trigger_types::CreaAttacks, triggers_creature[0]);
}

Planeswalker::Planeswalker(Card* src, Player* pl): Permanent(src, pl, 2), Damageable((int) src->get_flavor_text()[0], src){};

Damageable::Damageable(int lif, Card* source): life(lif){
	source->get_triggers(olympus::trigger_types::DamageableIsDamaged, is_damaged);
}

void Player::remove_permanent(Permanent* perm, int nb_zone){
	switch(nb_zone){
	    case 1: remove_permanent_inlist<Land>(&myboard.mylands, dynamic_cast<Land*>(perm)); break;
		case 2: remove_permanent_inlist<Artifact>(&myboard.myartos, dynamic_cast<Artifact*>(perm)); break;
		case 3: remove_permanent_inlist<Planeswalker>(&myboard.mysuperfriends, dynamic_cast<Planeswalker*>(perm)); break;
		case 4: remove_permanent_inlist<Creature>(&myboard.mycreas, dynamic_cast<Creature*>(perm)); break;
#ifdef NDEBUG
		default:
		god.gdebug(DBG_IMPORTANT) << "Error: Removed non-permanent" << std::endl;
		god.minimalKill();
		std::cout << "Error: Removed non-permanent" << std::endl;
		exit(1);
#endif
	}
}

template <class PermType>
void Player::remove_permanent_inlist(PContainer<PermType>* perms, PermType* asup){ //The source has already been sent to graveyard. This function only utterly deletes the Permanent.
	for(auto iter = perms->begin(); iter!=perms->end(); iter++){
		if(&(*iter) == asup){
			iter.get_pointed()->obliterate();
			return;
		}
	}
}

void Permanent::exile(){
	ctrl->puttozone(source, 2); //putting source in exile
	ctrl->remove_permanent(this, source->get_type());
}

void Permanent::destroy(){
	ctrl->puttozone(source, 1); //putting source in graveyard
	ctrl->remove_permanent(this, source->get_type());
}

bool Permanent::directactivate(){
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
}

void Permanent::activate(){
	if(nb_actabs == 1){
		if(first_actab[0].get_tapsymbol()) flags = flags & (~1);
		ctrl->manapool -= first_actab[0].cost;
		first_actab[0].cast_opt(ctrl);
	}
}

void Land::untap(){
	flags = flags | 1;
	ctrl->possiblepool += source->get_cost();
}

void Creature::hit(Damageable* tgt) const{
	tgt->damage(get_power(), (Target*) this);
}

void Planeswalker::activate() {
    thisturn = true;
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
    god.game->addtostack(new Resolvable(ctrl, loyalty_abs + x, (Target*) this));
}

typediterator<Permanent> BoardN::pbegin() {return !(mylands.empty()) ? mylands.pbegin() :
    !(myartos.empty()) ? myartos.pbegin() :
    !(mysuperfriends.empty()) ? mysuperfriends.pbegin() :
    !(mycreas.empty()) ? mycreas.pbegin() :
    typediterator<Permanent>();
}

typediterator<Permanent> BoardN::pend() const {return !(mycreas.empty()) ? mycreas.pend() :
    !(mysuperfriends.empty()) ? mysuperfriends.pend() :
    !(myartos.empty()) ? myartos.pend() :
    !(mylands.empty()) ? mylands.pend() :
    typediterator<Permanent>();
}

c_iterator<Permanent> BoardN::cpbegin() const {return !(mylands.empty()) ? mylands.cpbegin() :
    !(myartos.empty()) ? myartos.cpbegin() :
    !(mysuperfriends.empty()) ? mysuperfriends.cpbegin() :
    !(mycreas.empty()) ? mycreas.cpbegin() :
    c_iterator<Permanent>();
}

c_iterator<Permanent> BoardN::cpend() const {return !(mycreas.empty()) ? mycreas.cpend() :
    !(mysuperfriends.empty()) ? mysuperfriends.cpend() :
    !(myartos.empty()) ? myartos.cpend() :
    !(mylands.empty()) ? mylands.cpend() :
    c_iterator<Permanent>();
}

void BoardN::insert(Card *to_add, Player *pl) {
    switch(to_add->get_type()){
        case 1: mylands.construct_pure_child(to_add, pl); break;
        case 2: myartos.construct_pure_child(to_add, pl); break;
        case 3: mysuperfriends.construct_pure_child(to_add, pl); break;
        case 4: mycreas.construct_pure_child(to_add, pl); break;
        default: std::cout << "Internal error: trying to add non-permanent to battlefield." << std::endl; exit(1);
    }
}