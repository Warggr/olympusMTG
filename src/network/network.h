#ifndef OLYMPUS_NETWORK_H
#define OLYMPUS_NETWORK_H

#include <exception>
#include <istream>
#include <memory>
#include <unistd.h> //TODO DREAM this could be cross-platform

extern const char* id_client, * id_server, * version_server, * version_client;

enum operations { CREATE, UPDATE, DELETE };

class NetworkError: public std::exception {};

class Networker {
protected:
    static constexpr int BUFFER_SIZE = 100;
    char buffer[BUFFER_SIZE]{};
    int sockfd {-1};
    bool connected; //whether he is currently connected to that IP adress

    long read(); //Check if it was for closing , and also read the incoming message
public:
    static constexpr int PORT_NO = 4242;

    Networker(): sockfd(-1), connected(false) {};
    virtual ~Networker();

    void contact(const char* hostIp);

    void send(const std::string& message) const { send(message.c_str(), message.size()); }
    void send(const char* message, unsigned long size) const;
    void send_file(std::istream& file);
    uptr<std::istream> receive_file();
    int getSock() const { return sockfd; }
    bool isConnected() const { return connected; }

    virtual const char* getMessage(); //reads a C-style string from the network

    friend class BinaryBufferWriter;
};

#endif //OLYMPUS_NETWORK_H
