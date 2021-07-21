#include "networkclient.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

constexpr int MAX_NAME_LENGTH = 30;

NetworkClient::NetworkClient(const char* hostIp, const char* playerName) {
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
    std::cout << first_msg << "\n";
    int i = 0;
    for(i=0; id_server[i] != 0; i++) if(id_server[i] != first_msg[i]) {
        std::cout << "Doesn't look like anything. Rejected :(\n";
        throw NetworkError();
    }
    for(int j = 0; version_server[j] != 0; i++, j++) if(version_server[j] != buffer[i]) {
        std::cout << "Version mismatch: " << buffer + strlen(id_server) << " vs. " << version_server << "\n";
        throw NetworkError();
    }
}

NetworkClient::~NetworkClient() {
    close(sockfd);
}

void NetworkClient::reactToMessage(std::string message) {

}

[[noreturn]] void printUsageAndExit() {
    std::cout << "Usage: olympus_client HOST_IP_ADDR YOUR_NAME\n";
    exit(1);
}

int main(int argc, char** argv) {
    if(argc != 3) printUsageAndExit();
    NetworkClient client(argv[1], argv[2]);
    while(client.receive() != 0);
}

void NetworkClient::net_send(const std::string& message) const {
    long n = write(sockfd, message.c_str(), message.size());
    if(n < 0) throw NetworkError();
}

long NetworkClient::receive() {
    return read(sockfd, buffer, 255);
}

const char* NetworkClient::net_receive() {
    long n = read(sockfd, buffer, 255);
    if(n < 0) throw NetworkError();
    buffer[n] = 0;
    return buffer;
}
