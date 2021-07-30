#include "networkclient.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

constexpr int MAX_NAME_LENGTH = 30;

void NetworkClient::init(const char* hostIp, const char* playerName) {
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) throw NetworkError();

    in_addr addr;
    if(inet_aton(hostIp, &addr) < 0) throw NetworkError();

    struct sockaddr_in serv_addr = {
            .sin_family = AF_INET,
            .sin_port = htons(PORT_NO),
            .sin_addr = addr,
            .sin_zero = {0}
    };
    if (connect(sockfd, (sockaddr*) &serv_addr, sizeof(serv_addr)) < 0) throw NetworkError();

    net_send(std::string(id_client) + version_client + playerName);

    const char* first_msg = net_receive();
    int i = 0;
    for(i=0; id_server[i] != 0; i++) if(id_server[i] != first_msg[i]) {
        throw NetworkError();
    }
    for(int j = 0; version_server[j] != 0; i++, j++) if(version_server[j] != buffer[i]) {
        throw NetworkError();
    }
    connected = true;
}
