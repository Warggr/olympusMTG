#ifndef OLYMPUS_OLYMPUSCLIENT_H
#define OLYMPUS_OLYMPUSCLIENT_H

#include "network/client/networkclient.h"
#include "agents/local/localagent.h"
#include "gameplay/2cards.h"
#include "gameplay/gamer.h"
#include <boost/program_options.hpp>
#include <vector>
#include <list>
#include <unordered_map>

class CardWrapper;
using hand_type = std::list<CardWrapper>;

class OlympusClient {
    std::string playerName;
    LocalAgent agent;
    NetworkClient network;
    Deck deck;
    Gamer gamer;
    std::list<CardWrapper> hand;
    std::map<CardWrapper*, long long> wrapperMapping; //TODO OPTIMIZE
    void play();
    inline void discardCards(const char* message, long gcount);
    void drawCards(const char* message);
public:
    OlympusClient();
    void init(const boost::program_options::variables_map& vm);
    void create(const char* request);
    void start();
};

#endif //OLYMPUS_OLYMPUSCLIENT_H
