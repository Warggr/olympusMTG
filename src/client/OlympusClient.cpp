#include "OlympusClient.h"
#include "oracles/filereader/binary/binaryreader.h"
#include "network/protocol.h"
#include "logging.h"
#include <forward_list>
#include <memory>

OlympusClient::OlympusClient() {
    agent.getViewer().registerMe(this);
}

void OlympusClient::play() {
    auto request = network.receiveMessage_sync();
    switch(request[0]) {
        case operations::MESSAGE: agent.getViewer().message(request.c_str() + 1); break;
        case operations::CREATE: create(request.c_str() + 1); break;
        case operations::GET_OPTION: {
            bool sorceryspeed = static_cast<bool>(request[1]);
            const Option* opt = agent.chooseOpt(sorceryspeed, nullptr);
            long long ret;
            if(opt == nullptr)
                ret = 0;
            else
                ret = make_ref(opt);
            network.send(std::string_view(reinterpret_cast<char*>(&ret), sizeof(ret)));
        } break;
//        case 'U': frontEnd.update(request + 1); break;
//        case 'D': frontEnd.del(request + 1); break;
//        case 'B': frontEnd.bulkOp(request + 1); break;
        default:
            gdebug(DBG_NETWORK) << "Unrecognized opcode: " << request[0] << '\n';
    }
}

void OlympusClient::discardCards(const char* message, long gcount){
    (void) gcount;
    assert(message[0] == operations::CHOOSE_AMONG);
    const uint16_t size = *reinterpret_cast<const uint16_t*>(message + 1);
    const uint16_t nbToDiscard = *reinterpret_cast<const uint16_t*>(message + 2);
    assert(size == myHand.size());
    assert(size >= nbToDiscard);

    if(nbToDiscard != 0){
        auto discarded = agent.chooseCardsToKeep(myHand, nbToDiscard);
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
    uptr<std::istream> compiled_deck = network.receiveFile_sync();

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
        uptr<std::istream> hand_desc = network.receiveFile_sync();
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
        network.send(answer);
        if(keeps) break;
    }

    auto hand_str = network.receiveMessage_sync();
    std::string_view hand = hand_str;
    assert(hand[0] == operations::CREATE);
    assert(hand[1] == entities::CARDWRAPPER);
    drawCards(hand.data() + 2);

    auto discards_str = network.receiveMessage_sync();
    discardCards(discards_str.c_str(), discards_str.size());

    try {
        while (true) play();
    } catch(const UIClosedException& x) {

    }
}

void OlympusClient::drawCards(const char* message){
    constexpr int STEPSIZE = sizeof(long long) + sizeof(uint16_t);
    for(const char* i = message + 1; i < message + 1 + message[0] * STEPSIZE; i += STEPSIZE){
        long long llptr = *(reinterpret_cast<const long long*>(i));
        Card* card = deck.cards.data() + *(reinterpret_cast<const uint16_t*>(i + sizeof(long long)));
        CardWrapper& new_card = myHand.emplace_back(card, nullptr);
        auto ret = optionMapping.insert(std::make_pair(&new_card, llptr));
        assert(ret.second == true);
    }
}

void OlympusClient::create(const char* message) {
    switch(message[0]){
        case entities::CARDWRAPPER:
            drawCards(message + 1);
            break;
        default:
            gdebug(DBG_NETWORK) << "Unrecognized entity: " << message[0] << '\n';
    }
}
