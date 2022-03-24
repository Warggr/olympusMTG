#ifndef OLYMPUS_OLYMPUSCLIENT_H
#define OLYMPUS_OLYMPUSCLIENT_H

#include "network/client/networkclient.h"
#include "agents/local/localagent.h"
#include "gameplay/2cards.h"
#include <boost/program_options.hpp>
#include <list>

class CardWrapper;
using hand_type = std::list<CardWrapper>;

class OlympusClient {
    LocalAgent agent;
    NetworkClient network;
    Deck deck;
    hand_type hand;
    void play();
    inline void discardCards(const char* message, long gcount);
public:
    OlympusClient() = default;
    void init(const boost::program_options::variables_map& vm);
    void start();
};

#endif //OLYMPUS_OLYMPUSCLIENT_H
