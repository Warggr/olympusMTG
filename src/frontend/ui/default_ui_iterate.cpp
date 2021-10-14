#include "12defaultUI.h"
#include "lib3_IO.h"
#include "oracles/classes/8options.h"
#include "gameplay/1general.h"
#include "gameplay/optionwrappers.h"

Target* DefaultUI::iterate(bool needstarget, char returntypeflags) {
    return screen.iterate(returntypeflags, needstarget);
}

uptr<OptionAction> DefaultUI::chooseOpt(bool sorceryspeed) { //asks user to choose option and pops that option
	screen.fullDisp(myIO);
	return dynamic_cast<OptionWrapper*>(screen.iterate(target_type::optionable, sorceryspeed))
	->chooseOptionAction();
}
