#ifndef OLYMPUS_OLYMPUSCLIENT_H
#define OLYMPUS_OLYMPUSCLIENT_H

#include "network/client/networkclient.h"
#include "agents/local/localagent.h"
#include "gameplay/2cards.h"
#include "gameplay/gamer.h"
#include "gameplay/game.h"
#include <boost/program_options.hpp>
#include <vector>
#include <list>
#include <unordered_map>

class CardWrapper;
using hand_type = std::list<CardWrapper>;

class OlympusClient: public Gamer {
    std::string playerName;
    LocalAgent agent;
    NetworkClient network;
    Deck deck;
    ProtoGame game;
    std::map<const Option*, long long> optionMapping; //TODO OPTIMIZE
    void play();
    inline void discardCards(const char* message, long gcount);
    inline long long make_ref(const Option* opt){ return optionMapping.at(opt); }
    void drawCards(const char* message);
public:
    OlympusClient();
    void init(const boost::program_options::variables_map& vm);
    void create(const char* request);
    void start();
};

#endif //OLYMPUS_OLYMPUSCLIENT_H
