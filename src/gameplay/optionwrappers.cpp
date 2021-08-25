#include "optionwrappers.h"
#include "control/3player.h"
#include "agents/agent.h"
#include "cardinhand.h"
#include "oracles/classes/8options.h"
#include "oracles/classes/PermOption.h"

uptr<OptionAction> Permanent::chooseOptionAction() {
    return std::make_unique<PermOptionAction>(
            *options.at(ctrl->getAgent().chooseAmong(options)),
            *this
    );
}

uptr<OptionAction> CardInHand::chooseOptionAction() {
    return std::make_unique<SpellOptionAction>(
            *options.at(origin->getController()->getAgent().chooseAmong(options)),
            origin
    );
}
