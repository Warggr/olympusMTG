#ifndef OLYMPUS_AGENT_NETWORK_H
#define OLYMPUS_AGENT_NETWORK_H

#include "../agent.h"
#include "network.h"

class NetworkAgent: public Agent {
    bool idle; //whether he has been assigned a connection/IP adress yet
    bool connected; //whether he is currently connected to that IP adress
    bool unread; //whether any unread messages are in the buffer
    int sockfd;
    char buffer[1024];
    std::string name;
public:
	NetworkAgent();
	~NetworkAgent() override;

	void setup() override;
	int getSock() const { return sockfd; }
	void setSock(int sock);
    void receiveMessage();
    bool isConnected() const { return connected; }
    void setName(const char* name);
	std::string getName() override { return name; }
    //virtual Option* chooseOpt() override { return nullptr; }
    //virtual std::ifstream getDeck() { return };
    void net_send(const std::string& message) const;
    const char* net_receive();
};

#endif //OLYMPUS_AGENT_NETWORK_H
