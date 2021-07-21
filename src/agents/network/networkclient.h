#ifndef OLYMPUS_NETWORK_CLIENT_H
#define OLYMPUS_NETWORK_CLIENT_H

#include "network.h"
#include <string>

class NetworkClient {
    int sockfd;
    char buffer[1024];
public:
	NetworkClient(const char* hostip, const char* playerName);
	~NetworkClient();

	void reactToMessage(std::string message);
    void net_send(const std::string& message) const;
    long receive();
    const char* net_receive();
};

#endif //OLYMPUS_NETWORK_CLIENT_H