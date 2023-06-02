#ifndef OLYMPUS_ASYNC_H
#define OLYMPUS_ASYNC_H

#include "../network.h"
#include <boost/asio.hpp>
#include <queue>

using boost::system::error_code;

class NetworkManager;

class AsyncNetworker: public Networker {
public:
    enum state { FREE, CLAIMED, CONNECTED, INTERRUPTED_BY_SERVER };
protected:
    std::mutex protectReadingQueue;
    std::condition_variable signalReadingQueue;
    std::queue<std::string> reading_queue; //All messages that haven't been read yet
    bool listening = false;
    NetworkManager& server;
    enum state state = FREE;
    bool previouslyConnected = false;
    void on_connect(error_code ec);
    void on_write(error_code ec, std::size_t bytes_transferred);
    void on_read(error_code ec, std::size_t bytes_transferred);
    void fail(error_code ec, char const* what);

    std::condition_variable signalMessage;
    std::mutex protectBuffer;
    std::string name;

    void messageCallback();
    void await_packet() override;
public:
    AsyncNetworker(NetworkManager& server, tcp::socket socket): Networker(std::move(socket)), server(server) {};

    void setName(std::string_view name);
    const std::string& getName() { return name; }
    void setSock(tcp::socket sock);

    void await_reconnect();
    friend class NetworkManager;
};

#endif //OLYMPUS_ASYNC_H
