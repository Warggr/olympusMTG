#include "OlympusClient.h"
#include "oracles/filereader/binary/binaryreader.h"
#include "network/protocol.h"
#include "logging.h"
#include <forward_list>
#include <memory>

void OlympusClient::play() {
    const char* request = network.receiveMessage();
    switch(request[0]) {
        case operations::MESSAGE: agent.getViewer().message(request + 1); break;
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
    uptr<std::istream> compiled_deck = network.receiveFile();

    char header = compiled_deck->get();
    assert(header == operations::COMPILED_DECK);
    uint16_t size; compiled_deck->read(reinterpret_cast<char*>(&size), sizeof(size));

    deck.oracles = std::vector<CardOracle>(size);
    BinaryReader reader(*compiled_deck.get());
    for(auto& orc : deck.oracles) orc.init(reader);

    header = compiled_deck->get();
    assert(header == operations::COMPILED_DECK);
    compiled_deck->read(reinterpret_cast<char*>(&size), sizeof(size));

    deck.cards = std::vector<Card>(size);
    for(auto& card : deck.cards) {
        card.fromStr(*compiled_deck.get(), deck.oracles.data());
    }

    while(true) {
        uptr<std::istream> hand_desc = network.receiveFile();
        char a = hand_desc->get();
        assert(a == operations::CREATE);
        char nb = hand_desc->get();
        gdebug(DBG_NETWORK) << "Received hand: [CREATE][" << static_cast<int>(nb) << "]\n";
        std::forward_list<card_ptr> hand;
        BinaryReader reader(*hand_desc);
        for(int i=0; i<nb; i++){
            uint16_t offset; hand_desc->read(reinterpret_cast<char*>(&offset), sizeof(offset));
            gdebug(DBG_NETWORK) << offset << '\n';
            const Card* card = deck.cards.data() + offset;
            hand.push_front(card);
        }
        bool keeps = agent.keepsHand(hand);
        const char answer[] = {operations::KEEPS_HAND, static_cast<char>(keeps)};
        network.send(answer, sizeof(answer));
        if(keeps) break;
    };
    try {
        while (true) play();
    } catch(const UIClosedException& x) {

    }
}
