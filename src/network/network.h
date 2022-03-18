#ifndef OLYMPUS_NETWORK_H
#define OLYMPUS_NETWORK_H

#include <exception>
#include <istream>
#include <memory>
#include <unistd.h> //TODO DREAM this could be cross-platform

extern const char* id_client, * id_server, * version_server, * version_client;

enum operations { CREATE, UPDATE, DELETE };

class NetworkError: public std::exception {};

class Sender {
private:
    static constexpr unsigned int BUFFER_SIZE = 1000;
    int sockfd;
    unsigned int writehead;
    char buffer[BUFFER_SIZE]{};
    inline void send_chunks();
    bool try_add_chunk(std::istream& ifile);
public:
    static constexpr char MORE_PACKETS = '\n', END_OF_FILE = '\1';
    static_assert(MORE_PACKETS != END_OF_FILE);

    Sender(int sockfd): sockfd(sockfd), writehead(0) {};
    void add_chunk(const char* msg, unsigned int len);
    void add_chunk(const std::string& msg){ add_chunk(msg.c_str(), msg.size()); }
    void add_chunk(std::istream& ifile);
    void close();
};

class Networker {
protected:
    static constexpr int BUFFER_SIZE = 1000;
    char buffer[BUFFER_SIZE]{};
    int sockfd {-1};
    bool connected; //whether he is currently connected to that IP address

    long read(); //Check if it was for closing , and also read the incoming message
public:
    static constexpr int PORT_NO = 4242;

    Networker(): sockfd(-1), connected(false) {};
    virtual ~Networker();

    void contact(const char* hostIp);

    static long send(const char* message, unsigned long size, int fd);
    long send(const std::string& message) const { return send(message.c_str(), message.size(), sockfd); }
    long send(const char* message, unsigned long size) const { return send(message, size, sockfd); }
    void sendFile(std::istream& file);
    virtual const char* receiveMessage(); //reads a C-style string from the network
    uptr<std::istream> receiveFile();
    int getSock() const { return sockfd; }
    bool isConnected() const { return connected; }
    inline Sender getSender() const { return Sender(sockfd); }

    friend class BinaryBufferWriter;
};

#endif //OLYMPUS_NETWORK_H
