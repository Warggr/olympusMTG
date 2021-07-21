#include "networkagent.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "networkmanager.h"

NetworkAgent::NetworkAgent(): idle(true), connected(false) {
    NetworkManager::declareAgent(this);
}

NetworkAgent::~NetworkAgent() {
    if(connected) close(sockfd);
}

void NetworkAgent::setup() {
    NetworkManager::listener_mutex.lock();
    while(idle) { NetworkManager::listen(); }
    NetworkManager::listener_mutex.unlock();
}

void NetworkAgent::setSock(int sock) {
    idle = false; connected = true;
    sockfd = sock;
}

void NetworkAgent::receiveMessage() {
    //Check if it was for closing , and also read the incoming message
    long valread = read( sockfd , buffer, 1024);
    buffer[valread] = 0;
    if (valread == 0) { //Somebody disconnected , get his details and print
        sockaddr_in cli_addr;
        socklen_t clilen = sizeof(cli_addr);
        getpeername(sockfd, (sockaddr*) &cli_addr, &clilen);
        //printf("Host disconnected , ip %s , port %d \n", inet_ntoa(address.sin_addr) , ntohs(address.sin_port));

        //Close the socket
        close( sockfd );
        connected = false;
    }
    else unread = true;
}

void NetworkAgent::net_send(const std::string& message) const {
    long n = write(sockfd, message.c_str(), message.size());
    if(n < 0) throw NetworkError();
}

const char* NetworkAgent::net_receive() {
    while(!unread) { NetworkManager::listen(); }
    unread = false;
    return buffer;
}

void NetworkAgent::setName(const char *read_name) {
    name = std::string(read_name);
    std::cout << "Read name: " << name << "\n";
}
