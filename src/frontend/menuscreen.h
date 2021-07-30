#ifndef OLYMPUS_MENUSCREEN_H
#define OLYMPUS_MENUSCREEN_H

#include "lib3_IO.h"

class MenuScreen {
    ImplementIO& myIO;
    bool remote;
    std::string playerName, ipAdress;
public:
    MenuScreen(ImplementIO& io, bool is_remote);
    void main();
    std::string getPlayerName() const { return playerName; }
};

#endif //OLYMPUS_MENUSCREEN_H
