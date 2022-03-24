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

void OlympusClient::discardCards(const char* message, long gcount){
    assert(message[0] == operations::CHOOSE_AMONG);
    const uint16_t* size = reinterpret_cast<const uint16_t*>(message + 1);
    const uint16_t* nbToDiscard = size + 1;
    assert(*size >= *nbToDiscard);

    if(*nbToDiscard != 0){
        auto wrappers = std::list<CardWrapper>();
        for(const uint16_t* i = nbToDiscard + 1; reinterpret_cast<const char*>(i) < message + gcount - 1; i++){
            wrappers.emplace_front(&(deck.cards[*i]), nullptr);
        }
        auto discarded = agent.chooseCardsToKeep(wrappers, *nbToDiscard);
        Sender sender = network.getSender();
        char header = operations::CHOOSE_AMONG;
        sender.add_chunk(&header);
        for(auto& card : discarded){
            uint16_t offset = card.get() - deck.cards.data();
            sender.add_chunk(&offset);
        }
        sender.close();
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

        std::forward_list<card_ptr> hand;
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

    const char* discards = network.receiveMessage();
    discardCards(discards, network.gcount());

    try {
        while (true) play();
    } catch(const UIClosedException& x) {

    }
}
