#ifndef OLYMPUS_NETWORK_H
#define OLYMPUS_NETWORK_H

#include <exception>
#include <fstream>

constexpr int PORT_NO = 4242;

extern const char* id_client, * id_server, * version_server, * version_client;

class NetworkError: public std::exception {};

constexpr int BUFFER_SIZE = 50;

class Networker {
protected:
    char buffer[BUFFER_SIZE]{};
    int sockfd;
    bool connected; //whether he is currently connected to that IP adress

    void net_send(const std::string& message) const { net_send(message.c_str(), message.size()); }
    void net_send(const char* message, unsigned long size) const;
public:
    Networker(): sockfd(-1), connected(false) {};
    ~Networker();
    void send_file(std::ifstream& file);
    int getSock() const { return sockfd; }
    bool isConnected() const { return connected; }

    virtual const char* net_receive();
    virtual long receive();
};

#endif //OLYMPUS_NETWORK_H
