#ifndef OLYMPUS_NETWORKMANAGER_H
#define OLYMPUS_NETWORKMANAGER_H

#include "networkagent.h"
#include <memory>
#include <vector>
#include <netinet/in.h>
#include <string>
#include <mutex>

class NetworkManager {
private:
    static bool initialized;
    static int sockfd;
    static std::vector<NetworkAgent*> clients;
    static std::vector<NetworkAgent*> orphan_clients;
    static char buffer[256];

    static void initialize();
    static bool init_connection(int new_connection);
public:
    static std::mutex listener_mutex;
    static void declareAgent(NetworkAgent* agent);
    static void closeInstances();
    static void listen();
};

#endif //OLYMPUS_NETWORKMANAGER_H
