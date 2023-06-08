#include "networkagent.h"
#include "network/server/networkmanager.h"
#include "network/server/async.h"
#include "network/protocol.h"
#include "network/binary_data_reader.h"
#include "gameplay/2cards.h"
#include "oracles/filereader/visitor.h"
#include "oracles/filereader/binary/binarywriter.h"
#include <sstream>

NetworkAgent::NetworkAgent(NetworkManager& server): networker(server.declareAgent()) {}

void NetworkAgent::specificSetup() {
    networker.await_reconnect();
}

const Target* NetworkAgent::chooseTarget(char type) {
    (void) type; //TODO implement
    return nullptr;
}

void NetworkAgent::splitDamage(int power, std::list<std::pair<uint8_t, SpecificTargeter<Creature>>>& blockers) {
    (void) power; (void) blockers; //TODO implement
}

std::list<CardWrapper> NetworkAgent::chooseCardsToKeep(std::list<CardWrapper>& list, uint nbToDiscard) {
    assert(list.size() > nbToDiscard);

    Sender sender = networker.getSender();

    char header = operations::CHOOSE_AMONG; sender.add_chunk(&header);
    uint16_t size = list.size(); sender.add_chunk(&size);
    uint16_t nbSel = nbToDiscard; sender.add_chunk(&nbSel);
    sender.close(false);

    if(nbToDiscard == 0){
        return std::list<CardWrapper>();
    } else {
        auto ret = std::list<CardWrapper>();

        auto answer = networker.receiveMessage_sync();
        auto answer_data = BinaryDataReader(answer);
        assert(answer_data.get<char>() == operations::CHOOSE_AMONG);

        for(auto offset : BinaryDataArrayView<uint16_t>(answer_data.remainder()) ) {
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

    Sender sender = networker.getSender();

    char header = operations::COMPILED_DECK;
    sender.add_chunk(&header);
    uint16_t size = deck.oracles.size();
    sender.add_chunk(&size);

    for(auto& oracle : deck.oracles){
        std::stringstream oracle_stream;
        BinaryWriter oracle_reader(oracle_stream);
        visit<false>(oracle, oracle_reader);
        sender.add_chunk(oracle_stream.str());
    }

    header = operations::COMPILED_DECK;
    sender.add_chunk(&header);
    size = deck.cards.size();
    sender.add_chunk(&size);

    for(auto& card : deck.cards){
        sender.add_chunk(card.toStr(first_oracle));
    }

    sender.close(true);
}

bool NetworkAgent::keepsHand(const fwdlist<card_ptr>& cards) {
    const long size = 7;
    char header[2] = { operations::CREATE, static_cast<char>(size) };
    Sender sender = networker.getSender();
    sender.add_chunk(header, 2);

    int i = 0;
    for(auto iter = cards.begin(); i < size; ++i, ++iter ){
        uint16_t offset = (*iter) - first_card;
        sender.add_chunk(&offset);
    }
    sender.close(false);
    auto answer = networker.receiveMessage_sync();
    assert(answer[0] == operations::KEEPS_HAND);
    return (answer[1] == 1);
}

static_assert(reinterpret_cast<intptr_t>(nullptr) == 0); //assert that the agent never sends a zero for a valid pointer

const Option* NetworkAgent::chooseOpt(bool sorcerySpeed, const Player* pl) {
    (void) pl;
    char content[] = { operations::GET_OPTION, static_cast<char>(sorcerySpeed) };
    networker.send(std::string_view(content, sizeof(content)));

    auto msg = networker.receiveMessage_sync();
    assert(msg.size() == sizeof(long long));
    long long msg_long = *(reinterpret_cast<const long long*>(msg.c_str()));
    if(msg_long == 0) return nullptr;
    else return reinterpret_cast<const Option*>(msg_long);
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
    char header[] = { operations::CREATE, entities::CARDWRAPPER };
    Sender sender = networker.getSender();
    sender.add_chunk(header, sizeof(header));
    unsigned char length = cards.size();
    sender.add_chunk(&length);
    for(auto& wrapper: cards) {
        const Option* opt = &wrapper;
        intptr_t ptr = reinterpret_cast<intptr_t>(opt);
        long long llptr = ptr;
        uint16_t card = wrapper.get() - first_card;

        sender.add_chunk(&llptr);
        sender.add_chunk(&card);
    }
    sender.close();
}

uint NetworkAgent::chooseAmong(const std::vector<CardOption*>& opts) {
    (void) opts; return 0; //TODO
}

uint NetworkAgent::chooseAmong(const std::vector<PermOption>& opts) {
    (void) opts; return 0; //TODO
}

std::string NetworkAgent::getLogin() {
    return "127.0.0.1"; //value irrelevant - you're not supposed to chain two network agents
}

std::string NetworkAgent::getName() {
    return std::string(networker.getName());
}

uptr<std::istream> NetworkAgent::getDeckFile() {
    return networker.receiveFile_sync();
}

void NetworkAgent::registerMe(Gamer*) {}

void NetworkAgent::message(std::string_view message) {
    char header = operations::MESSAGE;
    Sender sender = networker.getSender();
    sender.add_chunk(&header, 1);
    sender.add_chunk(message);
    sender.close();
}
