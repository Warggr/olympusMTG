#include "OlympusClient.h"
#include "oracles/filereader/binary/binaryreader.h"
#include "network/protocol.h"
#include "network/binary_data_reader.h"
#include "agents/local/localagent.h"
#include "logging.h"
#include <iostream>

OlympusClient::OlympusClient() {
    agent.getViewer().registerMe(&player);
}

void OlympusClient::play() {
    auto request = network.receiveMessage_sync();
    auto request_data = BinaryDataReader(request);
    switch(request_data.get<char>()) {
        case operations::MESSAGE: agent.getViewer().message(request_data.remainder()); break;
        case operations::CREATE: create(request_data.remainder()); break;
        case operations::GET_OPTION: {
            bool sorcerySpeed = request_data.get<bool>();
            const Option* opt = agent.chooseOpt({ sorcerySpeed, &player });
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
            std::cerr << "Unrecognized opcode: " << request[0] << '\n';
    }
}

void OlympusClient::discardCards(std::string_view message){
    BinaryDataReader data(message);
    assert(data.get<char>() == operations::CHOOSE_AMONG);
    const auto size = data.get<uint16_t>();
    const auto nbToDiscard = data.get<uint16_t>();
    assert(size == player.getHand().size());
    assert(size >= nbToDiscard);
    assert(data.empty());

    if(nbToDiscard != 0){
        auto discarded = agent.chooseCardsToKeep(player.getHand(), nbToDiscard);
        Sender sender = network.getSender();
        char header = operations::CHOOSE_AMONG;
        sender.add_chunk(&header);
        for(auto& card : discarded){
            uint16_t offset = card.get() - deck.cards.data();
            sender.add_chunk(&offset);

            optionMapping.erase(&card);
        }
        sender.close(false);
    }
}

void OlympusClient::start() {
    uptr<std::istream> compiled_deck = network.receiveFile_sync();

    char header = compiled_deck->get();
    assert(header == operations::COMPILED_DECK);
    uint16_t size; compiled_deck->read(reinterpret_cast<char*>(&size), sizeof(size));

    deck.oracles = std::vector<CardOracle>(size);
    BinaryReader reader(*compiled_deck);
    for(auto& orc : deck.oracles) orc.init(reader);

    header = compiled_deck->get();
    assert(header == operations::COMPILED_DECK);
    compiled_deck->read(reinterpret_cast<char*>(&size), sizeof(size));

    deck.cards = std::vector<Card>(size);
    for(auto& card : deck.cards) {
        card.fromStr(*compiled_deck, deck.oracles.data());
    }

    while(true) {
        OwningReader<BinaryDataReader> hand_desc(network.receiveMessage_sync());
        char a = hand_desc.get<char>();
        assert(a == operations::CREATE);
        char nb = hand_desc.get<char>();

        std::forward_list<card_ptr> hand;
        for(int i=0; i<nb; i++){
            uint16_t offset = hand_desc.get<uint16_t>();
            gdebug(DBG_NETWORK) << offset << '\n';
            const Card* card = deck.cards.data() + offset;
            hand.push_front(card);
        }
        assert(hand_desc.empty());

        bool keeps = agent.keepsHand(hand);
        const char answer[] = {operations::KEEPS_HAND, static_cast<char>(keeps), Networker::END_OF_FILE};
        network.send(std::string_view(answer, 3));
        if(keeps) break;
    }

    auto hand = OwningReader<BinaryDataReader>(network.receiveMessage_sync());
    assert(hand.get<char>() == operations::CREATE);
    assert(hand.get<char>() == entities::CARDWRAPPER);
    drawCards(hand.remainder());

    auto discards_str = network.receiveMessage_sync();
    discardCards(discards_str);

    try {
        while (true) play();
    } catch(const UIClosedException& x) {

    }
}

void OlympusClient::drawCards(std::string_view message){
    auto data = BinaryDataReader(message);
    auto expected_length = data.get<unsigned char>();

    for(unsigned i = 0; i < expected_length; i++){
        long long llptr = data.get<long long>();
        uint16_t card_offset = data.get<uint16_t>();
        Card* card = deck.cards.data() + card_offset;
        CardWrapper& new_card = player.getHand().emplace_back(card, nullptr);
        Option* optionKey = &new_card;

        assert(not optionMapping.contains(optionKey));
        auto [_iter, success] = optionMapping.emplace(optionKey, llptr);
        assert(success);
    }
    assert(data.get<char>() == Networker::END_OF_FILE);
    assert(data.empty());
}

void OlympusClient::create(std::string_view message) {
    switch(message[0]){
        case entities::CARDWRAPPER:
            drawCards(message.substr(1));
            break;
        default:
            gdebug(DBG_NETWORK) << "Unrecognized entity: " << message[0] << '\n';
    }
}
