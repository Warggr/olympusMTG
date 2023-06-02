#include "async.h"
#include "networkmanager.h"
#include <iostream>

/* Callback function when a message has been received. */
void AsyncNetworker::messageCallback() {
    protectReadingQueue.lock();
    // TODO write message to queue
    protectReadingQueue.unlock();
    signalReadingQueue.notify_one();
}

void AsyncNetworker::setSock(tcp::socket newSocket) {
    this->socket = std::move(newSocket);
}

void AsyncNetworker::setName(std::string_view newName) {
    this->name = newName;
}

void AsyncNetworker::await_packet() {
    std::unique_lock lk(protectReadingQueue);
    if(reading_queue.empty()){
        signalReadingQueue.wait(lk, [&]{ return not reading_queue.empty(); });
    }
}

class DisconnectedException: std::exception {};
class TimeoutException: std::exception {};

void AsyncNetworker::await_reconnect() {
    std::unique_lock<std::mutex> lock(protectReadingQueue);
    if(state == INTERRUPTED_BY_SERVER) throw DisconnectedException();
    if(state == CONNECTED) return; //exit rapidly if the precondition isn't met

    std::cout << "(sync session) Awaiting reconnect...\n";
    using namespace std::chrono_literals;
    if(signalReadingQueue.wait_for(lock, 3min) == std::cv_status::timeout) throw TimeoutException();
    std::cout << "(sync session) ...reconnect signal heard\n";

    if(state == INTERRUPTED_BY_SERVER) throw DisconnectedException();

    assert(state == CONNECTED);
    std::cout << "(sync session) reconnected!\n";
}
