#include "oracles/filereader/visitor.h"
#include "network/protocol.h"
#include "networkagent.h"
#include "gameplay/2cards.h"
#include "oracles/filereader/binary/binarywriter.h"
#include <sstream>

NetworkAgent::NetworkAgent() = default;

void NetworkAgent::specificSetup() {
    networker.waitForConnection();
}

const Target* NetworkAgent::chooseTarget(char type) {
    (void) type; //TODO implement
    return nullptr;
}

void NetworkAgent::splitDamage(int power, std::list<std::pair<uint8_t, SpecificTargeter<Creature>>>& blockers) {
    (void) power; (void) blockers; //TODO implement
}

std::list<CardWrapper> NetworkAgent::chooseCardsToKeep(std::list<CardWrapper>& list, uint nbToDiscard) {
    Sender sender = networker.getSender();

    char header = operations::CHOOSE_AMONG; sender.add_chunk(&header);
    uint16_t size = list.size(); sender.add_chunk(&size);
    uint16_t nbSel = nbToDiscard; sender.add_chunk(&nbSel);

    if(nbToDiscard == 0){
        sender.close();
        return std::list<CardWrapper>();
    } else {
        for(auto& wrapper: list){
            const Card* card = wrapper.get();
            uint16_t offset = card - first_card;
            sender.add_chunk(reinterpret_cast<char*>(&offset), sizeof(offset));
        }
        sender.close();

        auto ret = std::list<CardWrapper>();

        const char* answer = networker.receiveMessage();
        assert(answer[0] == operations::CHOOSE_AMONG);

        for(int i = 1; i < networker.gcount() - 1; i += sizeof(uint16_t)){
            uint16_t offset = *reinterpret_cast<const uint16_t*>(answer + i);
            card_ptr card = first_card + offset;

            auto iter = list.begin();
            for( ; iter != list.end(); iter++ ){ //TODO OPTIMIZE
                if(iter->get() == card) {
                    ret.splice(iter, list);
                    break;
                }
            }
            assert(iter != list.end());
        }

        return ret;
    }
}

void NetworkAgent::connectDeck(const Deck& deck) {
    first_oracle = deck.oracles.data();
    first_card = deck.cards.data();

#define ADD_CHUNK_NOT_CHAR(x) add_chunk(reinterpret_cast<char*>(&x), sizeof(x));
#define ADD_CHUNK(x) add_chunk(&x, sizeof(x));
    Sender sender = networker.getSender();

    char header = operations::COMPILED_DECK;
    sender.ADD_CHUNK(header);
    uint16_t size = deck.oracles.size();
    sender.ADD_CHUNK_NOT_CHAR(size);

    for(auto& oracle : deck.oracles){
        std::stringstream oracle_stream;
        BinaryWriter oracle_reader(oracle_stream);
        visit<false>(oracle, oracle_reader);
        sender.add_chunk(oracle_stream.str());
    }

    header = operations::COMPILED_DECK;
    sender.ADD_CHUNK(header);
    size = deck.cards.size();
    sender.ADD_CHUNK_NOT_CHAR(size);

    for(auto& card : deck.cards){
        sender.add_chunk(card.toStr(first_oracle));
    }

    sender.close();
}

bool NetworkAgent::keepsHand(const fwdlist<card_ptr>& cards) {
    const long size = 7;
    char header[2] = { operations::CREATE, static_cast<char>(size) };
    Sender sender = networker.getSender();
    sender.add_chunk(header, 2);

    int i = 0;
    for(auto iter = cards.begin(); i < size; ++i, ++iter ){
        uint16_t offset = (*iter) - first_card;
        sender.add_chunk(reinterpret_cast<char*>(&offset), sizeof(offset));
    }
    sender.close();
    const char* answer = networker.receiveMessage();
    assert(answer[0] == operations::KEEPS_HAND);
    return (answer[1] == 1);
}

const Option* NetworkAgent::chooseOpt(bool sorcerySpeed, const Player* pl) {
    (void) sorcerySpeed; (void) pl; //TODO implement
    return nullptr;
}

void NetworkAgent::chooseAttackers(StateTN<Creature>& mycreas) {
    (void) mycreas; //TODO implement
}

void NetworkAgent::chooseBlockers(Y_Hashtable<Creature>& mycreas, StateTN<Creature>& attackers) {
    (void) mycreas; (void) attackers; //TODO implement
}

void NetworkAgent::connectGame(Game* gm) {
    game = gm;
}

void NetworkAgent::onDraw(const std::list<CardWrapper>& cards) {
    char header[2] = { operations::CREATE, static_cast<char>(cards.size()) };
    Sender sender = networker.getSender();
    sender.add_chunk(header, 2);
    for(auto& card: cards) {
        std::stringstream oracle_stream;
        BinaryWriter oracle_reader(oracle_stream);
        card->write(oracle_reader);
        sender.add_chunk(oracle_stream.str());
    }
    sender.close();
}

uint NetworkAgent::chooseAmong(const std::vector<CardOption*>& opts) {
    (void) opts; return 0; //TODO
}

uint NetworkAgent::chooseAmong(const std::vector<PermOption*>& opts) {
    (void) opts; return 0; //TODO
}

std::string NetworkAgent::getLogin() {
    return "127.0.0.1"; //value irrelevant - you're not supposed to chain two network agents
}

uptr<std::istream> NetworkAgent::getDeckFile() {
    return networker.receiveFile();
}

void NetworkAgent::registerMe(Player*) {}

void NetworkAgent::message(const char* message) {
    char header = operations::MESSAGE;
    Sender sender = networker.getSender();
    sender.add_chunk(&header, 1);
    uint i; for(i=0; message[i] != 0; ++i);
    sender.add_chunk(message, i);
    sender.close();
}
