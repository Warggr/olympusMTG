#include "OlympusClient.h"
#include "oracles/filereader/binary/binaryreader.h"
#include "logging.h"
#include <iostream>
#include <forward_list>
#include <memory>

void OlympusClient::play() {
    const char* request = network.receiveMessage();
    switch(request[0]) {
//        case 'X': frontEnd.create(request + 1); break;
//        case 'U': frontEnd.update(request + 1); break;
//        case 'D': frontEnd.del(request + 1); break;
//        case 'B': frontEnd.bulkOp(request + 1); break;
//        case '?': query(request + 1); break;
        default:
            gdebug(DBG_NETWORK) << "Unrecognized opcode: " << request[0] << '\n';
    }
}

void OlympusClient::start() {
    while(true) {
        uptr<std::istream> hand_desc = network.receiveFile();
        char a = hand_desc->get();
        assert(a == static_cast<char>(CREATE));
        char nb = hand_desc->get();
        std::cout << "Received hand: [CREATE][" << static_cast<int>(nb) << "]\n";
        for(int i = 0; i<10; i++){
            char a = hand_desc->get();
            std::cout << (a == 0 ? '_' : a);
        }
        std::cout << '\n';
        std::forward_list<std::unique_ptr<Card>> hand;
        BinaryReader reader(*hand_desc);
        for(int i=0; i<nb; i++){
            std::cout << "Read card starting with " << hand_desc->get() << '\n';
            std::shared_ptr<CardOracle> orc = std::make_shared<CardOracle>(reader);
            hand.push_front(std::make_unique<Card>(orc));
        }
        bool keeps = agent.keepsHand(hand);
        const char answer[] = {'H', 'K', static_cast<char>(keeps)};
        network.send(answer, 3);
        if(keeps) break;
    };
    try {
        while (true) play();
    } catch(const UIClosedException& x) {

    }
}
