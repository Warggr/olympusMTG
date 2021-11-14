#include "OlympusClient.h"
#include <iostream>
#include <unistd.h>

void OlympusClient::play() {
    /*const char* request = network.net_receive();
    switch(request[0]) {
        case 'X': frontEnd.create(request + 1); break;
        case 'U': frontEnd.update(request + 1); break;
        case 'D': frontEnd.del(request + 1); break;
        case 'B': frontEnd.bulkOp(request + 1); break;
        case '?': query(request + 1); break;
    }*/
}

void OlympusClient::query(const char *question) {
    (void) question; //TODO
}

void OlympusClient::start() {
    /*do {
        const char* hand = network.net_receive();
        frontEnd.create(hand);
    } while(frontEnd.askMulligan());
    try {
        while (true) play();
    } catch(const UIClosedException& x) {

    }*/
    while(true);
    //sleep(10);
}
