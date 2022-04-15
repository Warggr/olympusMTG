#include "async.h"
#include "networkmanager.h"
#include <iostream>

AsyncNetworker::AsyncNetworker() {
    NetworkManager::declareAgent(this);
}

const char* AsyncNetworker::receiveMessage() {
    NetworkManager::listener_mutex.lock();
    while(!unread) { NetworkManager::listen(); }
    NetworkManager::listener_mutex.unlock();
    unread = false;
    return buffer;
}

/* Callback function when a message has been received. */
void AsyncNetworker::messageCallback() {
    if(unread) throw NetworkError();
    Networker::receiveMessage();
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
    while(!connected) {
        NetworkManager::listen();
        std::cout << "Not connected\n";
    }
    NetworkManager::listener_mutex.unlock();
}
