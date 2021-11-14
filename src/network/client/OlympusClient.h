#ifndef OLYMPUS_OLYMPUSCLIENT_H
#define OLYMPUS_OLYMPUSCLIENT_H

#include "networkclient.h"
#include "agents/local/localagent.h"
#include <boost/program_options.hpp>

class OlympusClient {
    LocalAgent agent;
    NetworkClient network;
public:
    OlympusClient() = default;
    void init(const boost::program_options::variables_map& vm);
    void start();
    void play();
    void query(const char* question);
};

#endif //OLYMPUS_OLYMPUSCLIENT_H
