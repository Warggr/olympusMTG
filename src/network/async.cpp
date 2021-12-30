#include "async.h"
#include "network/networkmanager.h"
#include <iostream>

AsyncNetworker::AsyncNetworker() {
    NetworkManager::declareAgent(this);
}

const char* AsyncNetworker::receiveMessage() {
    while(!unread) { NetworkManager::listen(); }
    unread = false;
    //std::cout << "Server received " << buffer << "\n";
    return buffer;
}

/* Callback function when a message has been received. */
void AsyncNetworker::messageCallback() {
    read();
    unread = true;
}

void AsyncNetworker::setSock(int sock) {
    connected = true;
    sockfd = sock;
}

void AsyncNetworker::setName(const char *read_name) {
    name = std::string(read_name);
    std::cout << "Read name: " << name << "\n";
}

void AsyncNetworker::waitForConnection() {
    std::cout << "Waiting for connection...\n";
    NetworkManager::listener_mutex.lock();
    while(!connected) { NetworkManager::listen(); }
    NetworkManager::listener_mutex.unlock();
}
