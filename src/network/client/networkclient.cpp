#include "networkclient.h"
#include "network/protocol.h"
#include <boost/asio.hpp>
#include <iostream>

NetworkClient::NetworkClient(): Networker(tcp::socket(io_context)) {
}

void NetworkClient::init(tcp::endpoint server, const char* playerName) {
    boost::system::error_code ec;
    socket.connect(server, ec);

    networkThread = std::thread([&](){ io_context.run(); });

    if(ec){
        throw NetworkError();
    }
    assert(socket.is_open());

    send(std::string(id_client).append(version_client).append(playerName));

    auto first_msg = receiveMessage_sync();

    if(not first_msg.starts_with(id_server)){
        std::cerr << "Version mismatch: " << first_msg << ", expected " << id_server << '\n';
        throw NetworkError();
    } else first_msg = first_msg.substr(id_server.size());

    std::cout << first_msg << '\n';
    if(not first_msg.starts_with(version_server)){
        std::cerr << "Version mismatch: " << first_msg << ", expected " << version_server << '\n';
        throw NetworkError();
    }
}
