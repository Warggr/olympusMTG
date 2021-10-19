#include "optionwrappers.h"
#include "control/3player.h"
#include "agents/agent.h"
#include "cardinhand.h"
#include "oracles/classes/8options.h"
#include "oracles/classes/perm_option.h"

Option* Permanent::chooseOptionAction() {
    return options.at(ctrl->getAgent().chooseAmong(options));
}

Option* CardInHand::chooseOptionAction() {
    return options.at(origin->getController()->getAgent().chooseAmong(options));
}
