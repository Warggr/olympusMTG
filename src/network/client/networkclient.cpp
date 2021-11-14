#include "networkclient.h"

constexpr int MAX_NAME_LENGTH = 30;

void NetworkClient::init(const char* hostIp, const char* playerName) {
    contact(hostIp);

    send(std::string(id_client) + version_client + playerName);

    const char* first_msg = getMessage();
    int i;
    for(i=0; id_server[i] != 0; i++) if(id_server[i] != first_msg[i]) {
        throw NetworkError();
    }
    for(int j = 0; version_server[j] != 0; i++, j++) if(version_server[j] != first_msg[i]) {
        throw NetworkError();
    }
    connected = true;
}
