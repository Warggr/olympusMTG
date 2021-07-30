#ifndef OLYMPUS_OLYMPUSCLIENT_H
#define OLYMPUS_OLYMPUSCLIENT_H

#include "networkclient.h"
#include "frontend/frontend.h"

class OlympusClient {
    FrontEnd frontEnd;
    NetworkClient network;
public:
    OlympusClient() = default;
    void init(const char* hostIp, const char* playerName);
    void init() { init(frontEnd.getLogin().c_str(), frontEnd.getName().c_str()); }
    void start();
    void play();
    void query(const char* question);
};

#endif //OLYMPUS_OLYMPUSCLIENT_H
