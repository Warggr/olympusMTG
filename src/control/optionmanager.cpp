#include "optionmanager.h"
#include "3player.h"

bool OptionManager::hasOptions(bool sorceryspeed) const {
    (void) sorceryspeed;
    for(auto& card : player->myHand) {
        if(card->getType().land and player->canPlayLand()) {
            return true;
        } //TODO check for cheap non-land cards
    }

    return false;
}
