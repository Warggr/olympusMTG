#ifndef OLYMPUS_NETWORK_H
#define OLYMPUS_NETWORK_H

#include <exception>

constexpr int PORT_NO = 4242;

extern const char* id_client, * id_server, * version_server, * version_client;

class NetworkError: public std::exception {};

#endif //OLYMPUS_NETWORK_H
