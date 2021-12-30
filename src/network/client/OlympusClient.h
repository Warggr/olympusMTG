#ifndef OLYMPUS_OLYMPUSCLIENT_H
#define OLYMPUS_OLYMPUSCLIENT_H

#include "networkclient.h"
#include "agents/local/localagent.h"
#include <boost/program_options.hpp>

class OlympusClient {
    LocalAgent agent;
    NetworkClient network;
    std::map<int, Card> cards;
    void play();
public:
    OlympusClient() = default;
    void init(const boost::program_options::variables_map& vm);
    void start();

};

#endif //OLYMPUS_OLYMPUSCLIENT_H
