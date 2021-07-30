#ifndef OLYMPUS_AGENT_NETWORK_H
#define OLYMPUS_AGENT_NETWORK_H

#include "../agent.h"
#include "network.h"

class NetworkAgent: public Agent, public Networker {
    bool idle; //whether he has been assigned a connection/IP adress yet
    bool unread; //whether any unread messages are in the buffer
    long message_length;
    std::string name;

    const char* net_receive() override;
public:
	NetworkAgent();

	void specificSetup() override;
    void receiveMessage();

    void setName(const char* name);
	std::string getName() override { return name; }
    //virtual Option* chooseOpt() override { return nullptr; }
    void setSock(int sock);
    std::vector<OracleDescr> getDeck() override;
	Target* chooseTarget(char type) override;

    long receive() override;
};

#endif //OLYMPUS_AGENT_NETWORK_H
