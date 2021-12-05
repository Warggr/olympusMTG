#include "OutputManager.h"
#include "server/server.h"

void OutputManager::addToLog(const char *message) {
    for(auto& agent : Server::god->getAgents()) {
        agent->getViewer().message(message);
    }
}
