#ifndef OLYMPUS_ASYNC_H
#define OLYMPUS_ASYNC_H

#include "../network.h"

class AsyncNetworker: public Networker {
    bool unread;
    std::string name;

    void messageCallback();
public:
    AsyncNetworker();
    void waitForConnection();
    const char* receiveMessage() override;

    void setName(const char* name);
    const std::string& getName() { return name; }
    void setSock(int sock);
    friend class NetworkManager;
};

#endif //OLYMPUS_ASYNC_H
