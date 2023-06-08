#include "async.h"
#include "networkmanager.h"
#include "logging.h"
#include <iostream>

void AsyncNetworker::on_connect(tcp::socket&& socket, std::string&& name){
    {
        std::unique_lock lock(protectReadingQueue);

        assert(state == FREE);
        state = CONNECTED;
        this->socket = std::move(socket);
        this->name = std::move(name);
    }
    signalReadingQueue.notify_one();

    this->socket.async_read_some(getBufferIn(), [&](const boost::system::error_code& ec, std::size_t bytes_transferred){
        on_read(ec, bytes_transferred);
    });
}

void AsyncNetworker::on_read(const boost::system::error_code& ec, std::size_t bytes_transferred) {
    if(ec){
        std::cerr << "(async session) On async read: " << ec.message() << '\n';
    } else {
        std::unique_lock lock(protectReadingQueue);
        log_packet(false, std::string_view(buffer.data(), bytes_transferred));
        reading_queue.emplace(buffer.data(), bytes_transferred);
    }
    signalReadingQueue.notify_one();
    //Continue listening
    socket.async_read_some(getBufferIn(), [&](const boost::system::error_code& ec, std::size_t bytes_transferred){
        on_read(ec, bytes_transferred);
    });
}

std::string AsyncNetworker::receiveMessage_sync() {
    std::unique_lock lk(protectReadingQueue);
    if(reading_queue.empty()){
        signalReadingQueue.wait(lk, [&]{
            return not reading_queue.empty();
        });
    }
    auto result = std::move(reading_queue.front());
    reading_queue.pop();
    return result;
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
