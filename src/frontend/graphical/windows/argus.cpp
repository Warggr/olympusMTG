#include "argus.h"
#include "window.h"

#define LOGLEN 7

int Screen::linesize;

void Screen::initScreen(int width, int height) {
    coords = Rect(0, 0, width, height);
    int boardW = width - width / 4 - width / 5;

    board().init(Rect(0, 0, boardW, height));
    leftBar().init(Rect(board().coords.width, 0, width / 4, height));
    rightBar().init(Rect(leftBar().coords.right(), 0, width / 5, height));
}

void Board::init(const Rect& rect) {
    coords = rect;

    Rect child = coords;
    child.height = coords.height / 2 + Screen::linesize;
    players()[0].init(child);

    child.shift(0, coords.height / 2 - Screen::linesize);
    players()[1].init(child);
}

void LeftBar::init(const Rect& rect) {
    coords = rect;

    Rect child = rect;
    child.height = rect.height * 0.6;
    poster().coords = child;

    child.shift(0, child.height + 2*Screen::linesize); //there must be an upper-margin over the stack, for PreRes
    stack().coords = child;
}

void RightBar::init(const Rect& rect) {
    coords = rect;

    Rect child = rect;
    child.height -= (3+LOGLEN) * Screen::linesize;
    optionZone().init(child);

    child.shift(0, child.height);
    child.height = 3*Screen::linesize;
    messageZone().coords = child;

    child.shift(0, child.height);
    child.height = LOGLEN*Screen::linesize;
    logbookZone().coords = child;
}

void PlayerSide::init(const Rect& rect) {
    coords = rect;

    int playerH = 3 * (coords.height / 13); if(playerH < 2*Screen::linesize) playerH = 2*Screen::linesize;

    int permanentZSize = (coords.height - 2*playerH) / 6;
    if(permanentZSize < 3*Screen::linesize) permanentZSize = (coords.height - 2*playerH) / 5;

    int permanentZMargin = (coords.height - playerH - 5*permanentZSize) / 5;

    header().coords = Rect(rect.y, rect.z, rect.width, playerH);
    int startBoard = rect.z + playerH + permanentZMargin;
    board().policy.init(Rect(rect.y, startBoard, rect.width, rect.height - startBoard), permanentZSize, permanentZMargin);
}

void BoardSidePolicy::init(const Rect &rect, int permanentZSize, int permanentZMargin) {
    Rect child = rect;
    child.height = permanentZSize;
    for(BoardSidePolicy::iterator iter = begin(); iter != end(); ++iter) {
        iter->coords = child;
        child.shift(0, permanentZSize + permanentZMargin);
    }
}

void OptionZone::init(const Rect &rect) { coords = rect; }

OptionZone::OptionZone() {
    orientation = vertical;
    element_size = 2*Screen::linesize; offset = 0;
}
