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
	return (*pointed)->isCastable();
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
