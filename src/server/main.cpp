#include "networkagent.h"
#include <iostream>

[[ noreturn ]] void printUsageAndExit() {
    std::cout << "Usage: olympus_server [-n REMOTE_PLAYER_COUNT] [-l LOCAL_PLAYER_COUNT] [-b AI_PLAYER_COUNT]\n"
                 "where each PLAYER_COUNT is a single-digit number.\n"
                 "Please specify at least 2 players.\n";
    exit(1);
}

std::list<playerType> parseArgs(int nbargs, char** args) {
    std::list<playerType> list;
    int i = 1;
    playerType type;
    while(i != nbargs) {
        if(args[i][0] != '-' || i+1 == nbargs || args[i+1][0] > '9' || args[i+1][0] < '0' || args[i+1][1] != 0)
            printUsageAndExit();
        switch(args[i][1]) {
            case 'n': type = NETWORK; break;
            case 'l': type = LOCAL; break;
            //case 'b': type = BOT; break;
            default: printUsageAndExit();
        }
        std::cout << args[i+1][0] - '0' << " of type " << args[i][1] << ":";
        for(int j=0; j<(args[i+1][0] - '0'); j++) {
            list.push_front(type);
        }
        std::cout << list.size() << "\n";
        i += 2;
    }
    if(list.size() < 2) printUsageAndExit();
    return list;
}

int main(int nbargs, char** args) {
    std::list<playerType> l = parseArgs(nbargs, args);
    Server server;
    server.addPlayerX(l);
    server.launchGame();
}
