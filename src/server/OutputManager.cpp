#include "OutputManager.h"
#include "server/server.h"

void addToLog(const char *message) {
    for(auto& agent : Server::god->getAgents()) {
        agent->getViewer().message(message);
    }
}

void repLifeGain(Player* pl, int nbLife){
    (void) pl; (void) nbLife; //TODO
}
