#include "menuscreen.h"

MenuScreen::MenuScreen(ImplementIO& io, bool isRemote): myIO(io), remote(isRemote) {}

void MenuScreen::main() {
    myIO.setMenuScene();
    playerName = myIO.getTextInput("Your name:");
    if(remote) ipAdress = myIO.getTextInput("Server IP adress:");
}
