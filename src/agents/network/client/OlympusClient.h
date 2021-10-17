#ifndef OLYMPUS_OLYMPUSCLIENT_H
#define OLYMPUS_OLYMPUSCLIENT_H

#include "networkclient.h"
#include "../../local/localagent.h"

class OlympusClient {
    LocalAgent agent;
    NetworkClient network;
public:
    OlympusClient() = default;
    void init(const char* hostIp, const char* playerName);
    void init(const char* hostIp, const char* playerName, const char* fileName);
    void init();
    void start();
    void play();
    void query(const char* question);
};

#endif //OLYMPUS_OLYMPUSCLIENT_H
