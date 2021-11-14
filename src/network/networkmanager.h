#ifndef OLYMPUS_NETWORKMANAGER_H
#define OLYMPUS_NETWORKMANAGER_H

#include <memory>
#include <vector>
#include <netinet/in.h>
#include <string>
#include <mutex>

class AsyncNetworker;

class NetworkManager {
private:
    static constexpr int BUFFER_SIZE = 256;
    static bool initialized;
    static int sockfd;
    static std::vector<AsyncNetworker*> clients;
    static std::vector<AsyncNetworker*> orphan_clients;
    static char buffer[BUFFER_SIZE];

    static void initialize();
    static bool init_connection(int new_connection);
public:
    static std::mutex listener_mutex;
    static void declareAgent(AsyncNetworker* agent);
    static void closeInstances();
    static void listen();
};

#endif //OLYMPUS_NETWORKMANAGER_H
