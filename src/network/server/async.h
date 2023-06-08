#ifndef OLYMPUS_ASYNC_H
#define OLYMPUS_ASYNC_H

#include "../network.h"
#include <boost/asio.hpp>
#include <queue>

class NetworkManager;

class AsyncNetworker: public Networker {
public:
    enum state { FREE, CLAIMED, CONNECTED, INTERRUPTED_BY_SERVER };
protected:
    NetworkManager& server;

    std::string name;

    std::mutex protectReadingQueue;
    std::condition_variable signalReadingQueue;
    std::queue<std::string> reading_queue; //All messages that haven't been read yet
    bool listening = false;
    enum state state = FREE;
    bool previouslyConnected = false;

    void on_read(const boost::system::error_code& ec, std::size_t bytes_transferred);
    void on_connect(tcp::socket&& socket, std::string&& name);
public:
    AsyncNetworker(NetworkManager& server, tcp::socket socket): Networker(std::move(socket)), server(server) {};

    std::string_view getName() { return name; }

    void await_reconnect();

    std::string receiveMessage_sync() override;
    friend class NetworkManager;
};

#endif //OLYMPUS_ASYNC_H
