#include <catch2/catch_test_macros.hpp>
#include "network.h"
#include "async.h"
#include "networkmanager.h"
#include <thread>

TEST_CASE("Network", "[network]") {
    Networker client;
    AsyncNetworker server;

    std::thread parallel(&AsyncNetworker::waitForConnection, server);
    client.contact("127.0.0.1");
    parallel.join();

    std::string message = "Hello, world!";

    client.send(message);
    REQUIRE(server.receiveMessage() == message);

    message = "Hi, World!";
    server.send(message);
    REQUIRE(client.receiveMessage() == message);
}
