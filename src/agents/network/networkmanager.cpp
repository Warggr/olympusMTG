#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ifaddrs.h>
#include <cstring>
#include "networkmanager.h"
#include "networkagent.h"

void printMyIp();

void NetworkManager::initialize() {
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0) throw NetworkError();
    std::cout << "Starting a warggrserver on port " << PORT_NO << "...\nHere my IP(s):\n";
    printMyIp();

    int opt = 1;
    //set master socket to allow multiple connections, this is just a good habit, it will work without this
    if( setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char*) &opt, sizeof(opt)) < 0 ) throw NetworkError();

    struct sockaddr_in local_address = { .sin_family = AF_INET,
            .sin_port = htons(PORT_NO),
            .sin_addr = { .s_addr = INADDR_ANY } ,
            .sin_zero = {0}
    };

    if (bind(sockfd, (sockaddr*) &local_address, sizeof(local_address)) < 0) throw NetworkError();
    std::cout << "Socket open\n";
    ::listen(sockfd, 5);

    initialized = true;
}

void NetworkManager::closeInstances() {
    if(initialized) close(sockfd);
}

void NetworkManager::listen() {
    int max_fd = sockfd;
    fd_set readfds; //set of socket descriptors
    FD_ZERO(&readfds); //clear the socket set

    if(!orphan_clients.empty()) FD_SET(sockfd, &readfds); //add master socket to set
    for (NetworkAgent* agent : clients) if(agent->isConnected()) { //add child sockets to set
        if(agent->getSock() > max_fd) max_fd = agent->getSock();
        FD_SET( agent->getSock() , &readfds); //add to read list
    }

    //wait for an activity on one of the sockets , timeout is NULL , so wait indefinitely
    int activity = select( max_fd + 1 , &readfds , NULL , NULL , NULL);
    if ((activity < 0) && (errno!=EINTR)) throw NetworkError();

    //If something happened on the master socket ,
    //then its an incoming connection
    if (!orphan_clients.empty() && FD_ISSET(sockfd, &readfds)) {
        std::cout << "Orphans: " << orphan_clients.size() << "\n";
        sockaddr_in cli_addr;
        socklen_t clilen = sizeof(cli_addr);
        int new_socket = accept( sockfd, (sockaddr*) &cli_addr, (socklen_t*) &cli_addr);
        if (new_socket < 0) throw NetworkError();
        std::cout << "New socket open: " << new_socket << "\n";

        if(init_connection(new_socket)) {
            //inform user of socket number - used in send and receive commands
            std::cout << "New connection , socket fd is " << new_socket <<
                      " , ip is : " << inet_ntoa(cli_addr.sin_addr) <<
                      " , port : " << ntohs(cli_addr.sin_port) << "\n";
            NetworkAgent* agent = orphan_clients.back();
            agent->setSock(new_socket);
            agent->setName(buffer + strlen(id_client) + strlen(version_client));
            orphan_clients.pop_back();
        }
        else close(new_socket);
    }

    //else its some IO operation on some other socket
    for (NetworkAgent* agent : clients) if(agent->isConnected()) {
        if (FD_ISSET( agent->getSock(), &readfds)) {
            agent->receiveMessage();
        }
    }
}

bool NetworkManager::init_connection(int new_connection) {
    long n = read(new_connection, buffer, 255);
    if(n < 0) throw NetworkError();
    buffer[n] = 0;
    int i = 0;
    for(i=0; id_client[i] != 0; i++) if(id_client[i] != buffer[i]) return false;
    for(int j = 0; version_client[j] != 0; i++, j++) if(version_client[j] != buffer[i]) {
        std::cout << "Version mismatch: " << buffer + strlen(id_client) << " vs. " << version_client << "\n";
        return false;
    }

    std::string first_message = std::string(id_server) + version_server;
    n = write(new_connection, first_message.c_str(), first_message.size());
    if(n < 0) throw NetworkError();

    return true;
}

void printMyIp() {
    struct ifaddrs *all, *iter;
    getifaddrs(&all);
    iter = all;
    while (iter != nullptr) {
        if (iter->ifa_addr && iter->ifa_addr->sa_family == AF_INET) {
            auto *pAddr = (struct sockaddr_in *)iter->ifa_addr;
            std::cout << iter->ifa_name << ":" << inet_ntoa(pAddr->sin_addr) << "\n";
        }
        iter = iter->ifa_next;
    }
    freeifaddrs(all);
}

bool NetworkManager::initialized = false;
int NetworkManager::sockfd = 0;
std::vector<NetworkAgent*> NetworkManager::clients;
std::vector<NetworkAgent*> NetworkManager::orphan_clients;
char NetworkManager::buffer[256];
std::mutex NetworkManager::listener_mutex;