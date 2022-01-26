#include "argus.h"
#include "window.h"

#define LOGLEN 7

int Screen::linesize;

void Screen::initScreen(int width, int height) {
    setCoords(Rect(0, 0, width, height));
    int boardW = width - width / 4 - width / 5;

    board().setCoords(Rect(0, 0, boardW, height));
    leftBar().setCoords(Rect(board().getCoords().width, 0, width / 4, height));
    rightBar().setCoords(Rect(leftBar().getCoords().right(), 0, width / 5, height));
}

void Board::setCoords(const Rect& rect) {
    UIElement::setCoords(rect);

    Rect child = rect;
    child.height = rect.height / 2 + Screen::linesize;
    players()[0].setCoords(child);

    child.shift(0, rect.height / 2 - Screen::linesize);
    players()[1].setCoords(child);
}

void LeftBar::setCoords(const Rect& rect) {
    UIElement::setCoords(rect);

    Rect child = rect;
    child.height = rect.height * 0.6;
    poster().setCoords(child);

    child.shift(0, child.height + 2*Screen::linesize); //there must be an upper-margin over the stack, for PreRes
    stack().setCoords(child);
}

void RightBar::setCoords(const Rect& rect) {
    UIElement::setCoords(rect);

    Rect child = rect;
    child.height -= (3+LOGLEN) * Screen::linesize;
    optionZone().setCoords(child);

    child.shift(0, child.height);
    child.height = 3*Screen::linesize;
    messageZone().setCoords(child);

    child.shift(0, child.height);
    child.height = LOGLEN*Screen::linesize;
    logbookZone().setCoords(child);
}

void PlayerSide::setCoords(const Rect& rect) {
    UIElement::setCoords(rect);

    int playerH = 3 * (rect.height / 13); if(playerH < 2*Screen::linesize) playerH = 2*Screen::linesize;

    int permanentZSize = (rect.height - 2*playerH) / 6;
    if(permanentZSize < 3*Screen::linesize) permanentZSize = (rect.height - 2*playerH) / 5;

    int permanentZMargin = (rect.height - playerH - 5*permanentZSize) / 5;

    header().setCoords(Rect(rect.y, rect.z, rect.width, playerH));
    int startBoard = rect.z + playerH + permanentZMargin;
    board().policy.init(Rect(rect.y, startBoard, rect.width, rect.height - startBoard), permanentZSize, permanentZMargin);
}

void BoardSidePolicy::init(const Rect &rect, int permanentZSize, int permanentZMargin) {
    Rect child = rect;
    child.height = permanentZSize;
    for(BoardSidePolicy::iterator iter = begin(); iter != end(); ++iter) {
        iter->setCoords( child );
        child.shift(0, permanentZSize + permanentZMargin);
    }
}

OptionZone::OptionZone() {
    orientation = vertical;
    element_size = 2*Screen::linesize; offset = 0;
}
