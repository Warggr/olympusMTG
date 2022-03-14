#include "6abstractIO.h"
#include "gameplay/2cards.h"

void AbstractIO::draw(const CardWrapper& card, const Rect& rect, bool highlight) {
    draw(*card->oracle, rect, highlight);
}

BasicIO::checklistCallbackAction AbstractIO::getNextPosition(abstract_iterator_wrapper* iter, uint &position, uint max) {
    while(true) {
        DirectioL dir = get_direction_key();
        switch(dir) {
            case UP: case DOWN: return change;
            case ENTER: case BACK: return commit;
            case LEFT: if(position != 0) {
                    --position; iter->operator--();
                } break;
            case RIGHT: if(position != max) {
                    ++position; iter->operator++();
                } break;
            default: break;
        }
    }
}

void AbstractIO::disp(const CardOracle &oracle, int flags) const { disp_inrow(&oracle, 1, 1, flags); }
