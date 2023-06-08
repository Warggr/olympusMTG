#ifndef OLYMPUS_NETWORK_CLIENT_H
#define OLYMPUS_NETWORK_CLIENT_H

#include "network/network.h"
#include <boost/asio.hpp>

// using a class so that context (a member variable) can be used to initialize Networker (a parent class)
class _ContextWrapper {
public:
    std::thread networkThread;
    boost::asio::io_context io_context;
};

class NetworkClient: public _ContextWrapper, public Networker {
public:
    NetworkClient();
    void init(tcp::endpoint server, const char* playerName); //connects to the server. May take a long time.
};

#endif //OLYMPUS_NETWORK_CLIENT_H
