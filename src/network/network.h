#ifndef OLYMPUS_NETWORK_H
#define OLYMPUS_NETWORK_H

#include <boost/asio.hpp>
#include <exception>
#include <istream>
#include <string>
#include <array>
#include <memory>

namespace ip = boost::asio::ip;
using tcp = boost::asio::ip::tcp;

class NetworkError: public std::exception {};

/* Wraps a socket. Allows sending arbitrary amounts of data by buffering chunks. */
class Sender {
    tcp::socket& socket;
    unsigned int writehead;
    std::array<char, 1000> buffer;
    void send_chunks();
    bool try_add_chunk(std::istream& ifile);
public:
    Sender(tcp::socket& socket): socket(socket), writehead(0) {};

    void add_chunk_from_file(std::istream& ifile);

    void add_chunk(const char* chunk, std::size_t length);
    inline void add_chunk(std::string_view str){ add_chunk(str.data(), str.size()); }

    template<typename T>
    void add_chunk(T* chunk) { add_chunk(reinterpret_cast<char*>(chunk), sizeof(T) ); }
    void add_chunk(char* a) { add_chunk(a, 1); }

    void close();
};

/* This class contains both a socket and a buffer to contain the last message received.
 * This allows reading the message later from another thread. */
class Networker {
protected:
    tcp::socket socket;
    long gcount; //the last number of characters read, including the final 0
    std::array<char, 1000> buffer;

    // Wait for a packet to arrive. The default behavior is busy-wait,
    // but there's an override where we let the network loop send us the packet instead
    virtual void await_packet();
public:
    Networker(tcp::socket&& socket): socket(std::move(socket)) {};
    virtual ~Networker() = default;

    void send(std::string_view message);
    void sendFile(std::istream& file);
    Sender getSender() { return Sender(socket); }

    std::string receiveMessage_sync();
    uptr<std::istream> receiveFile_sync();
};

#endif //OLYMPUS_NETWORK_H
