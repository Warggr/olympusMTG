#ifndef OLYMPUS_NETWORK_CLIENT_H
#define OLYMPUS_NETWORK_CLIENT_H

#include "network/network.h"
#include <string>

class NetworkClient: public Networker {
public:
	void init(const char* hostip, const char* playerName); //connects to the server. May take a long time.
};

#endif //OLYMPUS_NETWORK_CLIENT_H
