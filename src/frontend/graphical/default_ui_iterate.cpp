#include "lib3_graphicalUI.h"
#include "oracles/classes/8options.h"
#include "gameplay/optionwrappers.h"

uptr<OptionAction> GraphicalUI::chooseOpt(bool sorceryspeed) { //asks user to choose option and pops that option
	screen.fullDisp(io);
	return dynamic_cast<OptionWrapper*>(screen.iterate(target_type::optionable, sorceryspeed))
	->chooseOptionAction();
}
