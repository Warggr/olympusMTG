#include "networkmanager.h"
#include "async.h"
#include "network/network.h"
#include "network/protocol.h"
#include <boost/asio.hpp>
#include <functional>
#include <iostream>

namespace net = boost::asio;
using tcp = net::ip::tcp;

const tcp::endpoint NetworkManager::default_endpoint = tcp::endpoint(boost::asio::ip::make_address("127.0.0.1"), 4242);

NetworkManager::NetworkManager(const tcp::endpoint& endpoint):
acceptor(context, endpoint)
{
    std::cout << "Starting a server on " << endpoint << "...\nHere my IP(s):\n";
    std::cout << endpoint;
    acceptor.async_accept(
        [&](const boost::system::error_code& error, tcp::socket peer) { on_connect(error, std::move(peer)); }
    ); //Start accepting connections
}

NetworkManager::~NetworkManager() {
}

void NetworkManager::on_connect(const boost::system::error_code& error, tcp::socket peer) {
    if(error){ std::cerr << "On connect: " << error << '\n'; return; }

    std::array<char, 1000> buffer;
    peer.async_read_some(boost::asio::buffer(buffer),
        [&buffer,peer=std::move(peer),this]
        (const boost::system::error_code& error, std::size_t bytes_transferred) mutable -> void {
            if(error){ std::cerr << "On read client hello: " << error << '\n'; return; }
            assert( buffer[bytes_transferred - 1] == '\0' ); // whole 0-terminated message transferred

            uint i;
            for(i=0; id_client[i] != 0; i++) if(id_client[i] != buffer[i]) return;
            for(uint j=0; version_client[j] != 0; i++, j++) if(version_client[j] != buffer[i]) {
                std::cerr << "Version mismatch: " << (&(buffer.front()) + id_client.size()) << " vs. " << version_client << "\n";
                return;
            }

            std::string first_message = std::string(id_server).append(version_server);
            boost::asio::const_buffer out_buffer(first_message.c_str(), first_message.size());
            peer.async_send(out_buffer, [](const boost::system::error_code& error, std::size_t bytes_transferred){
                (void) error; (void) bytes_transferred; // we don't care what happens once this message is sent
            });

            //inform user of socket number - used in send and receive commands
            std::cout << "New connection" <<
                      " , ip is : " << peer.remote_endpoint().address() <<
                      " , port : " << peer.remote_endpoint().port() << "\n";
            auto* agent = orphan_clients.back();
            orphan_clients.pop_back();
            agent->setSock(std::move(peer));
            const char* name = &buffer.front() + id_client.size() + version_client.size();
            std::size_t name_size = bytes_transferred - id_client.size() - version_client.size();
            agent->setName(std::string_view(name, name_size));
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
    auto newAgent = std::make_unique<AsyncNetworker>( *this, tcp::socket(context) );
    auto newAgent_ptr = newAgent.get();
    clients.push_back(std::move(newAgent));
    orphan_clients.push_back(newAgent_ptr);
    return *newAgent_ptr;
}
