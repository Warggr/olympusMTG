#include "localagent.h"
#include <fstream>

std::vector<OracleDescr> LocalAgent::getDeck() {
    std::vector<OracleDescr> ret;
    std::ifstream fb("material/decks/deck1.dck", std::ios::in);
    while(fb.peek() != EOF) {
        char buffer[1024]; int nb;
        fb >> nb >> std::skipws;
        fb.getline(buffer, 1024);
        ret.emplace_back(customcard, nb, std::string(buffer));
    }
    return ret;
}

Target *LocalAgent::chooseTarget(char type) {
    return nullptr; //TODO
}
