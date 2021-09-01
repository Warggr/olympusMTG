#include "frontend.h"

std::string FrontEnd::getLogin() {
    //switchContext(); //No need to switch context - there's only UI that might call AbstractUI::god
    return io.getTextInput("Your Name:");
}

std::string FrontEnd::getName() {
    //switchContext();
    return io.getTextInput("Server IP adress");
}

std::ifstream FrontEnd::getDeck() {
    //switchContext();
    std::ifstream ret;
    while(true) {
        std::string filename = io.getTextInput("Name of the deck file");
        ret.open(filename, std::ios::in);
        if(ret.is_open()) return ret;
        else io.message("Couldn't open file");
    }
}

void FrontEnd::splitDamage(int power, std::list<std::pair<uint8_t, SpecificTargeter<Creature>>> &blockers) {
    switchContext();
    (void) power; (void) blockers; //TODO
}
