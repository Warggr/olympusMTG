#ifndef OLYMPUS_NETWORK_H
#define OLYMPUS_NETWORK_H

#include <exception>
#include <istream>
#include <memory>

constexpr int PORT_NO = 4242;

extern const char* id_client, * id_server, * version_server, * version_client;

enum operations { CREATE, UPDATE, DELETE };

class NetworkError: public std::exception {};

class Networker {
protected:
    static constexpr int BUFFER_SIZE = 100;
    char buffer[BUFFER_SIZE]{};
    int sockfd {-1};
    bool connected; //whether he is currently connected to that IP adress

    void net_send(const std::string& message) const { net_send(message.c_str(), message.size()); }
    void net_send(const char* message, unsigned long size) const;
public:
    Networker(): sockfd(-1), connected(false) {};
    virtual ~Networker();
    void send_file(std::istream& file);
    uptr<std::istream> receive_file();
    int getSock() const { return sockfd; }
    bool isConnected() const { return connected; }

    virtual const char* net_receive();
    virtual long receive();

    friend class BinaryBufferWriter;
};

#endif //OLYMPUS_NETWORK_H
