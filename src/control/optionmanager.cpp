#include "optionmanager.h"
#include "3player.h"

bool OptionManager::hasOptions(bool sorceryspeed) const {
    for(auto& card : player->myHand) {
        if(card->getType().land and player->canPlayLand()) {
            return true;
        } //TODO check for cheap non-land cards
        (void) sorceryspeed;
        return false;
    }

    return false;
}
