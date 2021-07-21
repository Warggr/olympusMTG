#include "../HFiles/headC_constants.h"
#include "../HFiles/3player_options.h"
#include "../HFiles/4permanents.h"
#include "../HFiles/8options.h"
#include "../HFiles/14perm_options.h"
#include "../HFiles/6abstractUI.h"

template<typename OptT>
bool OptionWrapper<OptT>::pop() {
	if(_prev) _prev->_next = _next;
	if(_next){ _next->_prev = _prev; return false; }
	else return true;
}

OptionManager::OptionManager(){
	optZone = Implement_ui::god->get_optionzone();
}

OptionManager::~OptionManager(){
	/*Option* first = first_option(0);
	if(first) delete first;*/ //TODO
}

bool OptionManager::go_up(){
	gdebug(DBG_OPTIONS, "Requested to go up.\n");
	if(pointed != optionObjs.begin()){
		--pointed;
		return true;
	}
	else return false;
}

bool OptionManager::go_down(){
	gdebug(DBG_OPTIONS, "Requested to go down.\n");
	if(std::next(pointed, 1) != optionObjs.end()){
		pointed++;
		return true;
	}
	else return false;
}

bool OptionManager::has_options() const {
	if(!sorceryspeed and spells[0] == 0 and permOpts[0] == 0) return false;
	else if(spells[2] != 0) return true;
	else if(nbPlayedLands == 0 and !lands.empty()) return true;
	return false;
}

void OptionManager::disp_opts() const {
	int dy, dz;
	Rect rect = optZone->get_coordinates(&dy, &dz);
	bool not_moved = true;
	for(auto iter : optionObjs){
		iter->dispOpts(rect.y, rect.z, rect.width, not_moved, sorceryspeed);
		not_moved = false;
		rect.shift(dy, dz);
	}
}

void OptionManager::cast_pointed_opt(Player* pl){
	const Option& toCast = (*pointed)->chooseOpt();
	if(toCast.isLand()){
		++nbPlayedLands;
	}
	else{
		if(manaManager.pay(toCast.getCost(), 0)){
			if(STRICT_MODE) throw_invalid_arg_exception("Tried to pay an impossible option");
		}
	}
	toCast.cast_opt(pl);
}

void OptionManager::show_pointed(int y, int z, int width, bool highlight) const {
	(*pointed)->dispOpts(y, z, width, highlight, sorceryspeed);
}

/*PermOption* Permanent::activate(){ //TODO generalize PermOptions so it's usable 1:1 for PW abilities
	std::vector<std::string> vec(nb_actabs);
	std::vector<bool> castable(nb_actabs);
	int pos_castable = -1;
	for(unsigned short i=0; i<nb_actabs; i++){
		vec[i] = first_actab[i].describe();
		if(ctrl->myoptions.iscastable(&(first_actab[i]))){
			castable[i] = true;
			if(pos_castable == -1) pos_castable = i;
			else pos_castable = 0;
		}
		else castable[i] = false;
	} //check which abilities are castable and put them into a vector
	if(pos_castable == -1) return 0;
	else if(pos_castable != 0) return &(first_actab[pos_castable]);
	int i = god.myIO->choose_among(vec, castable);
	return &(first_actab[i]);
}*/

bool OptionManager::find_actabs(AbstractOptionObject* tar){
	//no idea what this was about *_*'
	return true;
}

void OptionManager::validate_in(Option* opt){
	//TODO
}

void OptionManager::validate_out(Option* opt){
	//TODO
}

bool OptionManager::pointed_is_castable() const {
	//gdebug(DBG_OPTIONS, "Castability test: metapos is " << metapos << "\n");
	/*if(metapos == LAND_OPT) return !hasplayedland;
	else*/ return (*pointed)->isCastable();
}

bool OptionManager::move(int nb_steps){
	gdebug(DBG_OPTIONS, "-Moving " << nb_steps << " steps-\n");
	if(nb_steps < 0)
		for(int i=0; i>nb_steps; i--){
			if(!go_down()) return false;
		}
	else
		for(int i=0; i<nb_steps; i++){
			if(!go_up()) return false;
		}
	return true;
}

/*void OptionManager::declare_option_collection(OptionHolder<PermOption>* all_options){
	for(unsigned i=0; i<all_options->nb_abilities; i++){
		if(all_options->abilities[i].quick_mana_effects != 0){
			ManaOption* manasource = all_options->abilities[i].quick_mana_effects;
			declare_mana_ability(manasource);
		}
		else{
			all_options->abilities[i].insert_before(options[PERMOPTION_OK]);
			options[PERMOPTION_OK] = &(all_options->abilities[i]); //inserting option in option chain
		}
	}
	check_too_expensive(PERMOPTION_BRANCH);
}*/
