#include "networkagent.h"
#include <iostream>
#include <filesystem>

[[ noreturn ]] void printUsageAndExit() {
    std::cout << "Usage: olympus_server [option(s)] [-n REMOTE_PLAYER_COUNT] [-l LOCAL_PLAYER_COUNT] [-b AI_PLAYER_COUNT]\n"
                 "where each PLAYER_COUNT is a single-digit number.\n"
                 "Please specify at least 2 players.\n";
    exit(1);
}

struct Usage {
    bool refresh_db{false}, start_game{false};
    uchar local{0}/*, bot, network*/;
};

Usage parseArgs(int nbargs, char** args) {
    Usage ret;
    int i = 1;
    while(i != nbargs) {
        if(args[i][0] != '-') printUsageAndExit();
        switch(args[i][1]) {
            //case 'n':
            //case 'b':
            case 'l': {
                if (i + 1 == nbargs || args[i + 1][0] > '9' || args[i + 1][0] < '0' || args[i + 1][1] != 0)
                    printUsageAndExit();
                uchar nb = args[i+1][0] - '0';
                switch(args[i][1]) {
                    //case 'n': ret.network += nb;
                    //case 'b': ret.bot += nb;
                    case 'l': ret.local += nb;
                }
                ++i;
            } break;
            case 'r': ret.refresh_db = true; break;
            default: printUsageAndExit();
        }
        i++;
    }
    uchar nb_players = ret.local /* + ret.bot + ret.network */;
    if(nb_players == 0) { if(!ret.refresh_db) printUsageAndExit(); }
    else if(nb_players < 2) printUsageAndExit();
    else ret.start_game = true;
    return ret;
}

int main(int nbargs, char** args) {
    std::filesystem::current_path("../material");

    Usage usage = parseArgs(nbargs, args);
    if(usage.refresh_db) refreshDatabase();
    if(!usage.start_game) return 0;

    std::list<playerType> types;
    for(int j=0; j<usage.local; j++) {
        types.push_front(LOCAL);
    }
    /*for(int j=0; j<usage.network; j++) {
        types.push_front(NETWORK);
    }
    for(int j=0; j<usage.bot; j++) {
        types.push_front(NOT);
    }*/

    Server server;
    server.addPlayers(types);
    server.launchGame();

    return 0;
}
