#ifndef OLYMPUS_NETWORKMANAGER_H
#define OLYMPUS_NETWORKMANAGER_H

#include <boost/asio.hpp>
#include <memory>
#include <vector>
#include <string>
#include <mutex>
#include <thread>

class AsyncNetworker;

using tcp = boost::asio::ip::tcp;

class NetworkManager {
private:
    boost::asio::io_context io_context;
    tcp::acceptor acceptor;
    std::vector<std::unique_ptr<AsyncNetworker>> clients;
    std::vector<AsyncNetworker*> orphan_clients;
    std::thread networkThread;

    void on_connect(const boost::system::error_code& error, tcp::socket&& peer);
    static const tcp::endpoint default_endpoint;
public:
    // Lock this mutex to make sure that the server doesn't change connection status while the main thread does something
    std::mutex protect_connections;

    NetworkManager(const tcp::endpoint& endpoint = default_endpoint);
    ~NetworkManager();
    AsyncNetworker& declareAgent();
};

#endif //OLYMPUS_NETWORKMANAGER_H
