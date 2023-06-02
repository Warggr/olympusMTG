#include "networkclient.h"
#include "network/protocol.h"
#include <boost/asio.hpp>

NetworkClient::NetworkClient(): Networker(tcp::socket(io_context)) {
}

void NetworkClient::init(tcp::endpoint server, const char* playerName) {
    boost::system::error_code ec;
    socket.connect(server, ec);

    if(ec){
        throw NetworkError();
    }

    send(std::string(id_client).append(version_client).append(playerName));

    auto first_msg = receiveMessage_sync();
    int i;
    for(i=0; id_server[i] != 0; i++) if(id_server[i] != first_msg[i]) {
        throw NetworkError();
    }
    for(int j = 0; version_server[j] != 0; i++, j++) if(version_server[j] != first_msg[i]) {
        throw NetworkError();
    }
}
