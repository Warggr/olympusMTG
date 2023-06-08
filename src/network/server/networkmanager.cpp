#include "networkmanager.h"
#include "async.h"
#include "network/network.h"
#include "network/protocol.h"
#include <boost/asio.hpp>
#include <iostream>
#include <memory>

namespace net = boost::asio;
using tcp = net::ip::tcp;

const tcp::endpoint NetworkManager::default_endpoint = tcp::endpoint(boost::asio::ip::make_address("127.0.0.1"), 4242);

NetworkManager::NetworkManager(const tcp::endpoint& endpoint):
acceptor(io_context, endpoint)
{
    std::cout << "Starting a server on " << endpoint << "...\n";
    acceptor.async_accept(
        [&](const boost::system::error_code& error, tcp::socket peer) {
            std::cout << "On connect (outside)" << std::endl; on_connect(error, std::move(peer));
        }
    ); //Start accepting connections

    networkThread = std::thread([&]{ std::cout << "ioc starting\n"; io_context.run(); std::cout << "ioc ending\n"; });
}

NetworkManager::~NetworkManager() {
    networkThread.join();
}

void NetworkManager::on_connect(const boost::system::error_code& error, tcp::socket&& peer) {
    if(error){ std::cerr << "On connect: " << error << '\n'; return; }

    std::cout << "On connect (inside)\n";
    assert(peer.is_open());

    auto buffer_ownership = std::string( 1000, '\0');
    auto buffer = boost::asio::buffer(buffer_ownership.data(), buffer_ownership.size());

    auto socket_ownership = std::make_unique<tcp::socket>(std::move(peer));
    auto socket = socket_ownership.get();

    socket->async_read_some(buffer,
        [buffer_ownership=std::move(buffer_ownership),socket_ownership=std::move(socket_ownership),this]
        (const boost::system::error_code& error, std::size_t bytes_transferred) mutable -> void {
            auto& peer = *socket_ownership;

            if(error){ std::cerr << "On read client hello: " << error.message() << '\n'; return; }
            std::cout << "On read client hello:" << bytes_transferred << "\n";
            std::string_view received(&buffer_ownership.front(), bytes_transferred);

            if(not received.starts_with(id_client)) {
                std::cerr << "Protocol mismatch: " << id_client << " vs. " << received << "\n";
                return;
            }
            received = received.substr(id_client.size());
            if(not received.starts_with(version_client)){
                std::cerr << "Version mismatch: " << received.substr(id_client.size()) << " vs. " << version_client << "\n";
                return;
            }
            received = received.substr(version_client.size());

            std::string first_message = std::string(id_server).append(version_server).append(1, Networker::END_OF_FILE);
            auto buffer = boost::asio::const_buffer(first_message.c_str(), first_message.size());

            peer.async_send(buffer, [](const boost::system::error_code& error, std::size_t bytes_transferred){
                std::cerr << "On server hello\n";
                if(error) std::cerr << "Warning: on server hello: " << error << '\n';
                std::cerr << bytes_transferred << " transferred\n";
                (void) bytes_transferred; // we don't care what happens once this message is sent
            });

            //inform user of socket number - used in send and receive commands
            std::cout << "New connection" <<
                      " , ip is : " << peer.remote_endpoint().address() <<
                      " , port : " << peer.remote_endpoint().port() << "\n";
            auto* agent = orphan_clients.back();
            orphan_clients.pop_back();
            agent->on_connect(std::move(*socket_ownership.release()), std::string(received));
        }
    );
    acceptor.async_accept(
        [&](const boost::system::error_code& error, tcp::socket peer) { on_connect(error, std::move(peer)); }
    ); //Continue accepting connections
}

void printMyIp() {
    // TODO FEATURE: print my IPs if available
    std::cout << "local" << ":" << "127.0.0.1" << "\n";
}

AsyncNetworker& NetworkManager::declareAgent() {
    auto newAgent = std::make_unique<AsyncNetworker>( *this, tcp::socket(io_context) );
    auto newAgent_ptr = newAgent.get();
    clients.push_back(std::move(newAgent));
    orphan_clients.push_back(newAgent_ptr);
    return *newAgent_ptr;
}
